/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/UKFCorrection.h>
#include <BayesFilters/utils.h>

using namespace bfl;
using namespace bfl::sigma_point;
using namespace Eigen;


UKFCorrection::UKFCorrection
(
    std::unique_ptr<MeasurementModel> measurement_model,
    const size_t n,
    const double alpha,
    const double beta,
    const double kappa
) noexcept :
    measurement_model_(std::move(measurement_model)),
    type_(UKFCorrectionType::Generic),
    ut_weight_(n, alpha, beta, kappa)
{ }


UKFCorrection::UKFCorrection
(
    std::unique_ptr<AdditiveMeasurementModel> measurement_model,
    const size_t n,
    const double alpha,
    const double beta,
    const double kappa
) noexcept :
    additive_measurement_model_(std::move(measurement_model)),
    type_(UKFCorrectionType::Additive),
    ut_weight_(n, alpha, beta, kappa)
{ }


UKFCorrection::UKFCorrection(UKFCorrection&& ukf_correction) noexcept :
    measurement_model_(std::move(ukf_correction.measurement_model_)),
    additive_measurement_model_(std::move(ukf_correction.additive_measurement_model_)),
    type_(ukf_correction.type_),
    ut_weight_(ukf_correction.ut_weight_)
{ }


MeasurementModel& UKFCorrection::getMeasurementModel()
{
    if (type_ == UKFCorrectionType::Additive)
        return *additive_measurement_model_;

    return *measurement_model_;
}


std::pair<bool, VectorXd> UKFCorrection::getLikelihood()
{
    if ((innovations_.rows() == 0) || (innovations_.cols() == 0))
        return std::make_pair(false, VectorXd());

    VectorXd likelihood(innovations_.cols());
    for (std::size_t i = 0; i < innovations_.cols(); i++)
    {
        likelihood(i) = utils::multivariate_gaussian_density(innovations_.col(i), VectorXd::Zero(innovations_.rows()), predicted_meas_.covariance(i)).coeff(0);
    }

    return std::make_pair(true, likelihood);
}


void UKFCorrection::correctStep(const GaussianMixture& pred_state, GaussianMixture& corr_state)
{
    /* Pick the correct measurement model. */
    MeasurementModel& model = getMeasurementModel();

    /* Get the current measurement if available. */
    bool valid_measurement;
    Data measurement;
    std::tie(valid_measurement, measurement) = model.measure();

    if (!valid_measurement)
    {
        corr_state = pred_state;
        return;
    }

    /* Initialize predicted measurement GaussianMixture. */
    std::pair<std::size_t, std::size_t> meas_sizes = model.getOutputSize();
    std::size_t meas_size = meas_sizes.first + meas_sizes.second;
    /* GaussianMixture will effectively resize only if it needs to. */
    predicted_meas_.resize(pred_state.components, meas_size);

    /* Evaluate the joint state-measurement statistics, if possible. */
    bool valid = false;
    MatrixXd Pxy;
    if (type_ == UKFCorrectionType::Generic)
    {
        /* Augment the previous state using measurement noise statistics. */
        GaussianMixture pred_state_augmented = pred_state;

        MatrixXd noise_covariance_matrix;
        std::tie(std::ignore, noise_covariance_matrix) = model.getNoiseCovarianceMatrix();
        pred_state_augmented.augmentWithNoise(noise_covariance_matrix);

        std::tie(valid, predicted_meas_, Pxy) = sigma_point::unscented_transform(pred_state_augmented, ut_weight_, *measurement_model_);
    }
    else if (type_ == UKFCorrectionType::Additive)
    {
        std::tie(valid, predicted_meas_, Pxy) = sigma_point::unscented_transform(pred_state, ut_weight_, *additive_measurement_model_);
    }

    if (!valid)
    {
        corr_state = pred_state;
        return;
    }

    /* Evaluate the innovation if possible. */
    bool valid_innovation;
    Data innovation;
    /* This temporary is required since some MeasurementModel::innovation methods may try to cast from
       const Ref<const MatrixXd> to MatrixXd resulting in a bfl::any::bad_any_cast.

       Hopefully, using std::move, it is possible to steal the memory from predicted_meas_.mean(). */
    MatrixXd y_p = std::move(predicted_meas_.mean());
    std::tie(valid_innovation, innovation) = model.innovation(y_p, measurement);

    if (!valid_innovation)
    {
        corr_state = pred_state;
        return;
    }

    /* Cast innovations once for all. */
    innovations_ = any::any_cast<MatrixXd&&>(std::move(innovation));

    /* Process all the components in the mixture. */
    for (size_t i=0; i < pred_state.components; i++)
    {
        /* Evaluate the Kalman Gain
           K = Pxy * (Py)^{-1} */
        MatrixXd K = Pxy.middleCols(meas_size * i, meas_size) * predicted_meas_.covariance(i).inverse();

        /* Evaluate the filtered mean.
           x_{k}+ = x{k}- + K * innovation */
        corr_state.mean(i) = pred_state.mean(i) + K * innovations_.col(i);

        /* Evaluate the filtered covariance
           P_{k}+ = P_{k}- - K * Py * K' */
        corr_state.covariance(i) = pred_state.covariance(i) - K * predicted_meas_.covariance(i) * K.transpose();
    }
}
