/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/DrawParticles.h>

#include <utility>

using namespace bfl;
using namespace Eigen;


DrawParticles::DrawParticles(std::unique_ptr<StateModel> state_model) noexcept :
    state_model_(std::move(state_model))
{ }


DrawParticles::DrawParticles(std::unique_ptr<StateModel> state_model, std::unique_ptr<ExogenousModel> exogenous_model) noexcept :
    state_model_(std::move(state_model)),
    exogenous_model_(std::move(exogenous_model))
{ }


DrawParticles::DrawParticles(DrawParticles&& prediction) noexcept :
    PFPrediction(std::move(prediction)),
    state_model_(std::move(prediction.state_model_)),
    exogenous_model_(std::move(prediction.exogenous_model_))
{ }


DrawParticles& DrawParticles::operator=(DrawParticles&& prediction) noexcept
{
    PFPrediction::operator=(std::move(prediction));

    state_model_ = std::move(prediction.state_model_);

    exogenous_model_ = std::move(prediction.exogenous_model_);

    return *this;
}


StateModel& DrawParticles::getStateModel() noexcept
{
    return *state_model_;
}


void DrawParticles::predictStep(const ParticleSet& prev_particles, ParticleSet& pred_particles)
{
    getStateModel().motion(prev_particles.state(), pred_particles.state());

    pred_particles.weight() = prev_particles.weight();
}
