/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef PFPREDICTIONDECORATOR_H
#define PFPREDICTIONDECORATOR_H

#include <BayesFilters/ParticleSet.h>
#include <BayesFilters/PFPrediction.h>

#include <memory>

namespace bfl {
    class PFPredictionDecorator;
}


class bfl::PFPredictionDecorator : public PFPrediction
{
protected:
    PFPredictionDecorator(std::unique_ptr<PFPrediction> prediction) noexcept;

    PFPredictionDecorator(PFPredictionDecorator&& prediction) noexcept;

    virtual ~PFPredictionDecorator() noexcept;

    PFPredictionDecorator& operator=(PFPredictionDecorator&& prediction) noexcept;

    StateModel& getStateModel() override;

    void setStateModel(std::unique_ptr<StateModel> state_model) override;

    ExogenousModel& getExogenousModel() override;

    void setExogenousModel(std::unique_ptr<ExogenousModel> exogenous_model) override;

protected:
    void predictStep(const bfl::ParticleSet& prev_particles, bfl::ParticleSet& pred_particles) override;

private:
    std::unique_ptr<PFPrediction> prediction_;
};

#endif /* PFPREDICTIONDECORATOR_H */
