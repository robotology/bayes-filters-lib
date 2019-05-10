/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef RESAMPLINGWITHPRIOR_H
#define RESAMPLINGWITHPRIOR_H

#include <BayesFilters/ParticleSet.h>
#include <BayesFilters/ParticleSetInitialization.h>
#include <BayesFilters/Resampling.h>

#include <memory>
#include <vector>

#include <Eigen/Dense>

namespace bfl {
    class ResamplingWithPrior;
}


class bfl::ResamplingWithPrior : public Resampling
{
public:
    ResamplingWithPrior(std::unique_ptr<bfl::ParticleSetInitialization> init_model, const double prior_ratio, const unsigned int seed) noexcept;

    ResamplingWithPrior(std::unique_ptr<bfl::ParticleSetInitialization> init_model, const double prior_ratio) noexcept;

    ResamplingWithPrior(std::unique_ptr<bfl::ParticleSetInitialization> init_model) noexcept;

    ResamplingWithPrior(ResamplingWithPrior&& resampling) noexcept;

    virtual ~ResamplingWithPrior() noexcept = default;

    ResamplingWithPrior& operator=(ResamplingWithPrior&& resampling) noexcept;

    void resample(const ParticleSet& cor_particles, ParticleSet& res_particles, Eigen::Ref<Eigen::VectorXi> res_parents) override;


protected:
    std::unique_ptr<bfl::ParticleSetInitialization> init_model_;

    double prior_ratio_ = 0.5;


private:
    std::vector<unsigned int> sort_indices(const Eigen::Ref<const Eigen::VectorXd>& vector);
};

#endif /* RESAMPLINGWITHPRIOR_H */
