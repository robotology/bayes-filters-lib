/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef UKFCORRECTION_H
#define UKFCORRECTION_H

#include <BayesFilters/AdditiveMeasurementModel.h>
#include <BayesFilters/GaussianCorrection.h>
#include <BayesFilters/GaussianMixture.h>
#include <BayesFilters/MeasurementModel.h>
#include <BayesFilters/sigma_point.h>

#include <Eigen/Dense>

namespace bfl {
    class UKFCorrection;
}


class bfl::UKFCorrection : public GaussianCorrection
{
public:
    UKFCorrection(std::unique_ptr<MeasurementModel> meas_model, const double alpha, const double beta, const double kappa, const bool update_weights_online = false) noexcept;

    UKFCorrection(std::unique_ptr<AdditiveMeasurementModel> meas_model, const double alpha, const double beta, const double kappa) noexcept;

    UKFCorrection(UKFCorrection&& ukf_prediction) noexcept;

    virtual ~UKFCorrection() noexcept = default;

    MeasurementModel& getMeasurementModel() override;

    std::pair<bool, Eigen::VectorXd> getLikelihood() override;

protected:
    void correctStep(const GaussianMixture& pred_state, GaussianMixture& corr_state) override;

    std::unique_ptr<MeasurementModel> measurement_model_;

    std::unique_ptr<AdditiveMeasurementModel> additive_measurement_model_;

    enum class UKFCorrectionType { Generic, Additive };

    /**
     * Distinguish between a UKFCorrection using a generic MeasurmentModel
     * and a UKFCorrection using an AdditiveMeasurementModel.
     */
    UKFCorrectionType type_;

    /**
     * Unscented transform weight.
     */
    sigma_point::UTWeight ut_weight_;

    /**
     * The unscented transform for a generic non-additive measurement model requires the computation of 2 * n + 1 sigma points
     * where n depends not only on the size of the input state but also on the size of the noise.
     * While the state size is fixed, the noise size might depend on the number of measurements available, hence n might change online.
     * Since the unscented transform weights, stored in ut_weight_, depend on n the user can specify, using the ctor input parameter
     * update_weights_online, that the weights have to be re-computed at each iteration.
     */
    const bool update_weights_online_ = false;

    const double ut_alpha_;

    const double ut_beta_;

    const double ut_kappa_;

    Eigen::MatrixXd innovations_;

    GaussianMixture predicted_meas_;
};

#endif /* UKFCORRECTION_H */
