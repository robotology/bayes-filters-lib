/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/Resampling.h>

#include <utility>

using namespace bfl;
using namespace Eigen;


Resampling::Resampling(unsigned int seed) noexcept :
    generator_(std::mt19937_64(seed))
{ }


Resampling::Resampling() noexcept :
    Resampling(1)
{ }


Resampling::~Resampling() noexcept
{ }


Resampling::Resampling(const Resampling& resampling) noexcept :
    generator_(resampling.generator_)
{ }


Resampling::Resampling(Resampling&& resampling) noexcept :
    generator_(std::move(resampling.generator_))
{ }


Resampling& Resampling::operator=(const Resampling& resampling)
{
    Resampling tmp(resampling);
    *this = std::move(tmp);

    return *this;
}


Resampling& Resampling::operator=(Resampling&& resampling) noexcept
{
    if (this == &resampling)
        return *this;

    generator_ = std::move(resampling.generator_);

    return *this;
}


Resampling& Resampling::operator=(const Resampling&& resampling) noexcept
{
    if (this == &resampling)
        return *this;

    generator_ = std::move(resampling.generator_);

    return *this;
}


void Resampling::resample(const ParticleSet& cor_particles, ParticleSet& res_particles,
                          Ref<VectorXi> res_parents)
{
    int num_particles = static_cast<int>(cor_particles.weight().rows());
    VectorXd csw(num_particles);

    csw(0) = exp(cor_particles.weight(0));
    for (int i = 1; i < num_particles; ++i)
        csw(i) = csw(i-1) + exp(cor_particles.weight(i));

    std::uniform_real_distribution<double> distribution_res(0.0, 1.0/num_particles);
    double u_1 = distribution_res(generator_);

    int idx_csw = 0;
    for (int j = 0; j < num_particles; ++j)
    {
        double u_j = u_1 + static_cast<double>(j)/num_particles;

        while (u_j > csw(idx_csw) && idx_csw < (num_particles - 1))
            idx_csw += 1;

        res_particles.state(j) = cor_particles.state(idx_csw);
        res_particles.mean(j) = cor_particles.mean(idx_csw);
        res_particles.covariance(j) = cor_particles.covariance(idx_csw);
        res_particles.weight(j) = -std::log(num_particles);
        res_parents(j) = idx_csw;
    }
}


double Resampling::neff(const Ref<const VectorXd>& cor_weights)
{
    return 1.0/cor_weights.array().exp().square().sum();
}
