/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef PFPREDICTION_H
#define PFPREDICTION_H

#include <BayesFilters/ExogenousModel.h>
#include <BayesFilters/ParticleSet.h>
#include <BayesFilters/StateModel.h>

#include <Eigen/Dense>

#include <memory>
#include <string>

namespace bfl {
    class PFPrediction;
}


class bfl::PFPrediction
{
public:
    virtual ~PFPrediction() noexcept = default;

    void predict(const ParticleSet& prev_particles, ParticleSet& pred_particles);

    bool skip(const std::string& what_step, const bool status);

    bool is_skipping();

    virtual StateModel& getStateModel() noexcept = 0;


protected:
    PFPrediction() noexcept = default;

    PFPrediction(const PFPrediction& prediction) noexcept = delete;

    PFPrediction& operator=(const PFPrediction& prediction) noexcept = delete;

    PFPrediction(PFPrediction&& prediction) noexcept = default;

    PFPrediction& operator=(PFPrediction&& prediction) noexcept = default;

    virtual void predictStep(const ParticleSet& prev_particles, ParticleSet& pred_particles) = 0;


private:
    bool skip_ = false;
};

#endif /* PFPREDICTION_H */
