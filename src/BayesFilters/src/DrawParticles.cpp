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


DrawParticles::DrawParticles() noexcept { }


DrawParticles::DrawParticles(DrawParticles&& draw_particles) noexcept :
    PFPrediction(std::move(draw_particles))
{ };


void DrawParticles::predictStep(const ParticleSet& prev_particles, ParticleSet& pred_particles)
{
    state_model_->motion(prev_particles.state(), pred_particles.state());

    pred_particles.weight() = prev_particles.weight();
}

StateModel& DrawParticles::getStateModel()
{
    return *state_model_;
}


void DrawParticles::setStateModel(std::unique_ptr<StateModel> state_model)
{
    state_model_ = std::move(state_model);
}
