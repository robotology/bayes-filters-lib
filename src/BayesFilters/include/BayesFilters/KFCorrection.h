/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef KFCORRECTION_H
#define KFCORRECTION_H

#include <BayesFilters/GaussianCorrection.h>
#include <BayesFilters/GaussianMixture.h>
#include <BayesFilters/LinearMeasurementModel.h>

#include <Eigen/Dense>

namespace bfl {
    class KFCorrection;
}


class bfl::KFCorrection : public GaussianCorrection
{
public:
    KFCorrection(std::unique_ptr<LinearMeasurementModel> measurement_model) noexcept;

    KFCorrection(KFCorrection&& kf_prediction) noexcept;

    virtual ~KFCorrection() noexcept = default;

    MeasurementModel& getMeasurementModel() override;

    std::pair<bool, Eigen::VectorXd> getLikelihood() override;

protected:
    void correctStep(const GaussianMixture& pred_state, GaussianMixture& corr_state) override;


private:
    std::unique_ptr<LinearMeasurementModel> measurement_model_;

    Eigen::MatrixXd innovations_;

    GaussianMixture meas_covariances_;
};

#endif /* KFCORRECTION_H */
