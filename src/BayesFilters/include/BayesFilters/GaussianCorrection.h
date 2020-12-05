/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef GAUSSIANCORRECTION_H
#define GAUSSIANCORRECTION_H

#include <BayesFilters/GaussianMixture.h>
#include <BayesFilters/MeasurementModel.h>

#include <Eigen/Dense>

namespace bfl {
    class GaussianCorrection;
}


class bfl::GaussianCorrection
{
public:
    virtual ~GaussianCorrection() noexcept = default;

    void correct(const GaussianMixture& pred_state, GaussianMixture& corr_state);

    bool skip(const bool status);

    virtual MeasurementModel& getMeasurementModel() = 0;

    bool freeze_measurements();

    virtual std::pair<bool, Eigen::VectorXd> getLikelihood();

protected:
    GaussianCorrection() noexcept;

    GaussianCorrection(GaussianCorrection&& gaussian_correction) noexcept;

    virtual void correctStep(const GaussianMixture& pred_state, GaussianMixture& corr_state) = 0;

private:
    bool skip_ = false;
};

#endif /* GAUSSIANCORRECTION_H */
