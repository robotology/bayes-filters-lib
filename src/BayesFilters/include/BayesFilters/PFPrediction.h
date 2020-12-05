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

    bool getSkipState();

    bool getSkipExogenous();

    virtual void setStateModel(std::unique_ptr<StateModel> state_model) = 0;

    virtual void setExogenousModel(std::unique_ptr<ExogenousModel> exogenous_model);

    virtual StateModel& getStateModel() = 0;

    virtual ExogenousModel& getExogenousModel();

protected:
    PFPrediction() noexcept;

    PFPrediction(PFPrediction&& pf_prediction) noexcept;

    virtual void predictStep(const ParticleSet& prev_particles, ParticleSet& pred_particles) = 0;

private:
    bool skip_prediction_ = false;

    bool skip_state_ = false;

    bool skip_exogenous_ = false;
};

#endif /* PFPREDICTION_H */
