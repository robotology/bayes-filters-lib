/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef GPFPREDICTION_H
#define GPFPREDICTION_H

#include <BayesFilters/ExogenousModel.h>
#include <BayesFilters/GaussianPrediction.h>
#include <BayesFilters/ParticleSet.h>
#include <BayesFilters/PFPrediction.h>
#include <BayesFilters/StateModel.h>

#include <memory>

namespace bfl {
    class GPFPrediction;
}


class bfl::GPFPrediction : public PFPrediction
{
public:
    GPFPrediction(std::unique_ptr<GaussianPrediction> gauss_prediction) noexcept;

    GPFPrediction(const GPFPrediction& prediction) noexcept = delete;

    GPFPrediction& operator=(const GPFPrediction& prediction) noexcept = delete;

    GPFPrediction(GPFPrediction&& prediction) noexcept;

    GPFPrediction& operator=(GPFPrediction&& prediction) noexcept;

    virtual ~GPFPrediction() noexcept = default;

    StateModel& getStateModel() noexcept override;


protected:
    void predictStep(const ParticleSet& previous_particles, ParticleSet& predicted_particles) override;

    std::unique_ptr<GaussianPrediction> gaussian_prediction_;
};

#endif /* GPFPREDICTION_H */
