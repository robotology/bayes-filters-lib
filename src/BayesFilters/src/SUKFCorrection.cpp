/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/SUKFCorrection.h>
#include <BayesFilters/directional_statistics.h>

using namespace bfl;
using namespace bfl::directional_statistics;
using namespace bfl::sigma_point;
using namespace Eigen;


SUKFCorrection::SUKFCorrection
(
    std::unique_ptr<AdditiveMeasurementModel> measurement_model,
    const size_t n,
    const double alpha,
    const double beta,
    const double kappa,
    const size_t measurement_sub_size
) noexcept :
    SUKFCorrection(std::move(measurement_model), n, alpha, beta, kappa, measurement_sub_size, false)
{ }


SUKFCorrection::SUKFCorrection
(
    std::unique_ptr<AdditiveMeasurementModel> measurement_model,
    const size_t n,
    const double alpha,
    const double beta,
    const double kappa,
    const size_t measurement_sub_size,
    const bool use_reduced_noise_covariance_matrix
) noexcept :
    measurement_model_(std::move(measurement_model)),
    ut_weight_(n, alpha, beta, kappa),
    measurement_sub_size_(measurement_sub_size),
    use_reduced_noise_covariance_matrix_(use_reduced_noise_covariance_matrix)
{ }


SUKFCorrection::SUKFCorrection(SUKFCorrection&& sukf_correction) noexcept :
    measurement_model_(std::move(sukf_correction.measurement_model_)),
    ut_weight_(sukf_correction.ut_weight_),
    measurement_sub_size_(sukf_correction.measurement_sub_size_),
    use_reduced_noise_covariance_matrix_(sukf_correction.use_reduced_noise_covariance_matrix_)
{ }


MeasurementModel& SUKFCorrection::getMeasurementModel()
{
    return *measurement_model_;
}


void SUKFCorrection::correctStep(const GaussianMixture& pred_state, GaussianMixture& corr_state)
{
    /* Get the current measurement if available. */
    bool valid_measurement;
    Data measurement;
    std::tie(valid_measurement, measurement) = measurement_model_->measure();

    /* Check if the size of the measurement is compatible with measurement_sub_size. */
    std::pair<std::size_t, std::size_t> sizes = measurement_model_->getOutputSize();
    std::size_t meas_size = sizes.first + sizes.second;
    valid_measurement &= (((meas_size) % measurement_sub_size_ ) == 0);

    if (!valid_measurement)
    {
        corr_state = pred_state;
        return;
    }

    /* Sample sigma points. */
    MatrixXd input_sigma_points = sigma_point::sigma_point(pred_state, ut_weight_.c);

    /* Propagate sigma points. */
    Data pred;
    bool valid_pred;
    std::tie(valid_pred, pred) = measurement_model_->predictedMeasure(input_sigma_points);

    if (!valid_pred)
    {
        corr_state = pred_state;
        return;
    }

    /* Cast data to MatrixXd. */
    MatrixXd prop_sigma_points = bfl::any::any_cast<MatrixXd&&>(std::move(pred));

    /* Evaluate the predicted mean. */
    std::size_t size_sigmas = (pred_state.dim * 2) + 1;
    MatrixXd pred_mean(meas_size, pred_state.components);
    for (size_t i = 0; i < pred_state.components; i++)
    {
        Ref<MatrixXd> prop_sp = prop_sigma_points.middleCols(size_sigmas * i, size_sigmas);

        /* Evaluate the mean. */
        pred_mean.col(i).noalias() = prop_sp * ut_weight_.mean;
    }

    /* Evaluate the innovation if possible. */
    bool valid_innovation;
    Data innovation;
    std::tie(valid_innovation, innovation) = measurement_model_->innovation(pred_mean, measurement);

    if (!valid_innovation)
    {
        corr_state = pred_state;
        return;
    }

    /* Cast innovations once for all. */
    MatrixXd innovations = any::any_cast<MatrixXd&&>(std::move(innovation));

    /* From now on using equations from the paper:
       Barfoot, T., McManus, C. (2011),
       'A Serial Approach to Handling High-Dimensional Measurements in the Sigma-Point Kalman Filter.',
       Science and Systems VII,
       MIT Press */

    /* Process all the components in the mixture. */
    MatrixXd sqrt_ut_weight = ut_weight_.covariance.array().sqrt().matrix().asDiagonal();
    for (size_t i = 0; i < pred_state.components; i++)
    {
        /* Compose square root of the measurement covariance matrix.
           IV.C.2.b */
        Ref<MatrixXd> Y = prop_sigma_points.middleCols(size_sigmas * i, size_sigmas);

        /* Shift w.r.t. the mean. */
        Y.colwise() -= pred_mean.col(i);

        /* Weight using square root of unscented transform weight. */
        Y *= sqrt_ut_weight;

        /* Compose matrix C and vector d by cycling over all the sub-vector of the measurement
           IV.C.2.c */
        MatrixXd C_inv = MatrixXd::Identity(size_sigmas, size_sigmas);
        VectorXd d = VectorXd::Zero(size_sigmas);

        for (size_t j = 0; j < (meas_size) / measurement_sub_size_; j++)
        {
            MatrixXd tmp(size_sigmas, measurement_sub_size_);
            tmp.noalias() = Y.middleRows(measurement_sub_size_ *j, measurement_sub_size_).transpose() * getNoiseCovarianceMatrix(j).inverse();

            C_inv += tmp * Y.middleRows(measurement_sub_size_ *j, measurement_sub_size_);

            d += tmp * innovations.col(i).middleRows(measurement_sub_size_ *j, measurement_sub_size_);
        }

        /* Process input sigma points.
           IV.C.3 */
        Ref<MatrixXd> X = input_sigma_points.middleCols(size_sigmas * i, size_sigmas);
        X.topRows(pred_state.dim_linear).colwise() -= pred_state.mean(i).topRows(pred_state.dim_linear);
        X.bottomRows(pred_state.dim_circular) = directional_sub(X.bottomRows(pred_state.dim_circular), pred_state.mean(i).bottomRows(pred_state.dim_circular));
        X *= sqrt_ut_weight;

        C_inv = C_inv.inverse();

        /* Evaluate the filtered mean.
           IV.C.4 */
        corr_state.mean(i) = pred_state.mean(i) + X * C_inv * d;

        /* Evaluate the filtered covariance.
           IV.C.4 */
        corr_state.covariance(i) = X * C_inv * X.transpose();
    }
}


MatrixXd SUKFCorrection::getNoiseCovarianceMatrix(const std::size_t index)
{
    /* Obtain the noise covariance matrix from the measurement model. */
    MatrixXd R;
    std::tie(std::ignore, R) = measurement_model_->getNoiseCovarianceMatrix();

    if (use_reduced_noise_covariance_matrix_)
        return R;
    else
        return R.block(measurement_sub_size_ * index, measurement_sub_size_ * index, measurement_sub_size_, measurement_sub_size_);
}


std::pair<bool, Eigen::VectorXd> SUKFCorrection::likelihood(const Eigen::Ref<const Eigen::MatrixXd>& innovations)
{
    throw std::runtime_error("ERROR::SUKFCORRECTION::LIKELIHOOD\nERROR:\n\tMethod not implemented.");
}
