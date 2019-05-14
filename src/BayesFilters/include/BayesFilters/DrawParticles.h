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

namespace bfl {
    class DrawParticles;
}


class bfl::DrawParticles : public PFPrediction
{
public:
    DrawParticles(std::unique_ptr<StateModel> state_model) noexcept;

    DrawParticles(std::unique_ptr<StateModel> state_model, std::unique_ptr<ExogenousModel> exogenous_model) noexcept;

    DrawParticles(const DrawParticles& prediction) noexcept = delete;

    DrawParticles& operator=(const DrawParticles& prediction) noexcept = delete;

    DrawParticles(DrawParticles&& prediction) noexcept;

    DrawParticles& operator=(DrawParticles&& prediction) noexcept;

    virtual ~DrawParticles() noexcept = default;

    StateModel& getStateModel() noexcept override;


protected:
    void predictStep(const ParticleSet& prev_particles, ParticleSet& pred_particles) override;

    std::unique_ptr<StateModel> state_model_;

    std::unique_ptr<ExogenousModel> exogenous_model_;
};

#endif /* DRAWPARTICLES_H */
