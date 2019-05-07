/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/ResamplingWithPrior.h>
#include <BayesFilters/utils.h>

#include <algorithm>
#include <numeric>
#include <vector>

using namespace bfl;
using namespace Eigen;


ResamplingWithPrior::ResamplingWithPrior(std::unique_ptr<bfl::ParticleSetInitialization> init_model, const double prior_ratio, const unsigned int seed) noexcept :
    Resampling(seed),
    init_model_(std::move(init_model)),
    prior_ratio_(prior_ratio)
{ }


ResamplingWithPrior::ResamplingWithPrior(std::unique_ptr<ParticleSetInitialization> init_model, const double prior_ratio) noexcept :
    Resampling(1),
    init_model_(std::move(init_model)),
    prior_ratio_(prior_ratio)
{ }


ResamplingWithPrior::ResamplingWithPrior(std::unique_ptr<ParticleSetInitialization> init_model) noexcept :
    Resampling(1),
    init_model_(std::move(init_model))
{ }


ResamplingWithPrior::ResamplingWithPrior(ResamplingWithPrior&& resampling) noexcept :
    Resampling(std::move(resampling)),
    init_model_(std::move(resampling.init_model_))
{
    prior_ratio_ = resampling.prior_ratio_;
    resampling.prior_ratio_ = 0.5;
}


ResamplingWithPrior::~ResamplingWithPrior() noexcept
{ }


ResamplingWithPrior& ResamplingWithPrior::operator=(ResamplingWithPrior&& resampling) noexcept
{
    if (this != &resampling)
    {
        Resampling::operator=(std::move(resampling));

        init_model_ = std::move(resampling.init_model_);
    }

    return *this;
}


void ResamplingWithPrior::resample(const ParticleSet& cor_particles, ParticleSet& res_particles, Ref<VectorXi> res_parents)
{
    int num_prior_particles    = static_cast<int>(std::floor(cor_particles.state().cols() * prior_ratio_));
    int num_resample_particles = cor_particles.state().cols() - num_prior_particles;

    /* Consider two subsets of particles. */
    ParticleSet res_particles_left(num_prior_particles, cor_particles.dim_linear, cor_particles.dim_circular);
    ParticleSet res_particles_right(num_resample_particles, cor_particles.dim_linear, cor_particles.dim_circular);
    Ref<VectorXi> res_parents_right(res_parents.tail(num_resample_particles));

    /* Copy particles to be resampled in a temporary. */
    ParticleSet tmp_particles(num_resample_particles, cor_particles.dim_linear, cor_particles.dim_circular);
    int j = 0;
    for (std::size_t i : sort_indices(cor_particles.weight().array().exp()))
    {
        if (j >= num_prior_particles)
        {
            tmp_particles.state(j - num_prior_particles) = cor_particles.state(i);
            tmp_particles.mean(j - num_prior_particles) = cor_particles.mean(i);
            tmp_particles.covariance(j - num_prior_particles) = cor_particles.covariance(i);
            tmp_particles.weight(j - num_prior_particles) = cor_particles.weight(i);
        }
        j++;
    }

    /* Normalize weights using LogSumExp. */
    tmp_particles.weight().array() -= utils::log_sum_exp(tmp_particles.weight());

    /* Resample from tmp_particles. */
    Resampling::resample(tmp_particles, res_particles_right, res_parents_right);
    res_parents_right.array() += num_prior_particles;

    /* Initialize from scratch num_prior_particles particles. */
    init_model_->initialize(res_particles_left);

    /* Merge the two subset together. */
    res_particles = std::move(res_particles_left + res_particles_right);

    /* Reset weights.*/
    res_particles.weight().setConstant(-std::log(cor_particles.state().cols()));

    /* Since num_prior_particles were created from scratch,
       they do not have a parent. */
    res_parents.head(num_prior_particles).array() = -1;
}


std::vector<unsigned int> ResamplingWithPrior::sort_indices(const Ref<const VectorXd>& vector)
{
    std::vector<unsigned int> idx(vector.size());
    std::iota(idx.begin(), idx.end(), 0);

    std::sort(idx.begin(), idx.end(),
              [&vector](size_t idx1, size_t idx2) { return vector[idx1] < vector[idx2]; });

    return idx;
}
