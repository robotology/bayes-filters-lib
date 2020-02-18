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


GPFPrediction::GPFPrediction(std::unique_ptr<GaussianPrediction> gauss_pred) noexcept :
    gaussian_prediction_(std::move(gauss_pred))
{ }


GPFPrediction::GPFPrediction(GPFPrediction&& gpf_prediction) noexcept :
    PFPrediction(std::move(gpf_prediction)),
    gaussian_prediction_(std::move(gpf_prediction.gaussian_prediction_))
{ }


StateModel& GPFPrediction::getStateModel()
{
    return gaussian_prediction_->getStateModel();
}


void GPFPrediction::setStateModel(std::unique_ptr<StateModel> state_model)
{
    throw std::runtime_error("ERROR::GPFPREDICTION::GETSTATEMODEL\nERROR:\n\tCall to unimplemented base class method.");
}


void GPFPrediction::predictStep(const bfl::ParticleSet& prev_particles, bfl::ParticleSet& pred_particles)
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
