/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef DRAWPARTICLES_H
#define DRAWPARTICLES_H

#include <BayesFilters/ParticleSet.h>
#include <BayesFilters/PFPrediction.h>

#include <random>
#include <memory>

namespace bfl {
    class DrawParticles;
}


class bfl::DrawParticles : public PFPrediction
{
public:
    DrawParticles() noexcept;

    DrawParticles(DrawParticles&& draw_particles) noexcept;

    virtual ~DrawParticles() noexcept = default;

    virtual StateModel& getStateModel() override;

    virtual void setStateModel(std::unique_ptr<StateModel> state_model) override;


protected:
    void predictStep(const ParticleSet& prev_particles, ParticleSet& pred_particles) override;

    std::unique_ptr<StateModel> state_model_;
};

#endif /* DRAWPARTICLES_H */
