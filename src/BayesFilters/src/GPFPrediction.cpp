/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include "BayesFilters/GPFPrediction.h"

#include <exception>

using namespace bfl;
using namespace Eigen;


GPFPrediction::GPFPrediction(std::unique_ptr<GaussianPrediction> gauss_prediction) noexcept :
    gaussian_prediction_(std::move(gauss_prediction))
{ }


GPFPrediction::GPFPrediction(GPFPrediction&& prediction) noexcept :
    PFPrediction(std::move(prediction)),
    gaussian_prediction_(std::move(prediction.gaussian_prediction_))
{ }


GPFPrediction& GPFPrediction::operator=(GPFPrediction&& prediction) noexcept
{
    PFPrediction::operator=(std::move(prediction));

    gaussian_prediction_ = std::move(prediction.gaussian_prediction_);

    return *this;
}


StateModel& GPFPrediction::getStateModel() noexcept
{
    return gaussian_prediction_->getStateModel();
}


void GPFPrediction::predictStep(const ParticleSet& prev_particles, ParticleSet& pred_particles)
{
    /* Set skip flags within the Gaussian prediction. */
    gaussian_prediction_->skip("state", getSkipState());
    gaussian_prediction_->skip("exogenous", getSkipExogenous());

    /* Propagate Gaussian belief associated to each particle. */
    gaussian_prediction_->predict(prev_particles, pred_particles);

    /* Copy particles weights since they do not change. */
    pred_particles.weight() = prev_particles.weight();

    /* Copy position of particles since they do not change.

       Note: in this step the mean of the Gaussian belief of each particle is updated
       while the position of the particles do not change. */
    pred_particles.state() = prev_particles.state();

}
