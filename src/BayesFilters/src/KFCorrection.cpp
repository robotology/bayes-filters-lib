/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/KFCorrection.h>
#include <BayesFilters/utils.h>

using namespace bfl;
using namespace Eigen;


KFCorrection::KFCorrection(std::unique_ptr<LinearMeasurementModel> measurement_model) noexcept :
    measurement_model_(std::move(measurement_model))
{ }


KFCorrection::KFCorrection(KFCorrection&& kf_correction) noexcept :
    measurement_model_(std::move(kf_correction.measurement_model_))
{ }


MeasurementModel& KFCorrection::getMeasurementModel()
{
    return *measurement_model_;
}


std::pair<bool, VectorXd> KFCorrection::getLikelihood()
{
    if ((innovations_.rows() == 0) || (innovations_.cols() == 0))
        return std::make_pair(false, VectorXd());

    VectorXd likelihood(innovations_.cols());
    for (std::size_t i = 0; i < likelihood.size(); i++)
    {
        likelihood(i) = utils::multivariate_gaussian_density(innovations_.col(i), VectorXd::Zero(innovations_.rows()), meas_covariances_.covariance(i)).coeff(0);
    }

    return std::make_pair(true, likelihood);
}


void KFCorrection::correctStep(const GaussianMixture& pred_state, GaussianMixture& corr_state)
{
    /* Get the current measurement if available. */
    bool valid_measurement;
    Data measurement;
    std::tie(valid_measurement, measurement) = measurement_model_->measure();

    if (!valid_measurement)
    {
        corr_state = pred_state;
        return;
    }

    /* Evaluate the predicted measurement if possible. */
    bool valid_predicted_measurement;
    Data predicted_measurement;
    std::tie(valid_predicted_measurement, predicted_measurement) = measurement_model_->predictedMeasure(pred_state.mean());

    if (!valid_predicted_measurement)
    {
        corr_state = pred_state;
        return;
    }

    /* Evaluate the innovation if possible. */
    bool valid_innovation;
    Data innovation;
    std::tie(valid_innovation, innovation) = measurement_model_->innovation(predicted_measurement, measurement);

    if (!valid_innovation)
    {
        corr_state = pred_state;
        return;
    }

    bool valid_covariance_matrix;
    MatrixXd R;
    std::tie(valid_covariance_matrix, R) = measurement_model_->getNoiseCovarianceMatrix();

    if (!valid_covariance_matrix)
    {
        corr_state = pred_state;
        return;
    }

    MatrixXd H = measurement_model_->getMeasurementMatrix();

    /* Cast innovations once for all. */
    innovations_ = any::any_cast<MatrixXd&&>(std::move(innovation));

    /* Initialize measurement covariances.
       GaussianMixture will effectively resize only if it needs to. */
    meas_covariances_.resize(pred_state.components, H.rows());

    /* Process all the components in the mixture. */
    for (size_t i = 0; i < pred_state.components; i++)
    {
        /* Evaluate the measurement covariance matrix
           Py = H * Px * H' + R */
        meas_covariances_.covariance(i) = H * pred_state.covariance(i) * H.transpose() + R;

        /* Evaluate the Kalman Gain
           K = Px * H' * (Py)^{-1} */
        MatrixXd K = pred_state.covariance(i) * H.transpose() * meas_covariances_.covariance(i).inverse();

        /* Evaluate the filtered mean
           x_{k}+ = x{k}- + K * (y - y_predicted) */
        corr_state.mean(i) = pred_state.mean(i) + K * innovations_.col(i);

        /* Evaluate the filtered covariance
           P_{k}+ = P_{k}- - K * Py * K' */
        corr_state.covariance(i).noalias() = pred_state.covariance(i) - K * meas_covariances_.covariance(i) * K.transpose();
    }
}
