/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/ParticleSet.h>

using namespace bfl;
using namespace Eigen;

ParticleSet::ParticleSet() noexcept :
    ParticleSet(1, 1, 0, false)
{ }


ParticleSet::ParticleSet(const std::size_t components, const std::size_t dim) noexcept:
    ParticleSet(components, dim, 0, false)
{ }


ParticleSet::ParticleSet
(
    const std::size_t components,
    const std::size_t dim_linear,
    const std::size_t dim_circular,
    const bool use_quaternion
) noexcept :
    GaussianMixture(components, dim_linear, dim_circular, use_quaternion),
    state_(dim, components)
{ }


ParticleSet::~ParticleSet() noexcept
{ }


void ParticleSet::resize(const std::size_t components, const std::size_t dim_linear, const std::size_t dim_circular)
{
    std::size_t new_dim = dim_linear + dim_circular * dim_circular_component;

    if ((this->dim_linear == dim_linear) && (this->dim_circular = dim_circular) && (this->components == components))
        return;
    else if ((this->dim == new_dim) && (this->components != components))
        state_.conservativeResize(NoChange, components);
    else
    {
        // In any other case, it does not make sense to do conservative resize
        // since either old data is truncated or new data is incomplete
        state_.resize(new_dim, components);
    }

    GaussianMixture::resize(components, dim_linear, dim_circular);
}


ParticleSet& ParticleSet::operator+=(const ParticleSet& rhs)
{
    /* Should check whether (this->dim_linear == rhs.dim_linear) &&
       (this->dim_circular == rhs.dim_circular). */
    std::size_t new_components = components + rhs.components;

    state_.conservativeResize(NoChange,  new_components);
    state_.rightCols(rhs.components) = rhs.state_;

    mean_.conservativeResize(NoChange,  new_components);
    mean_.rightCols(rhs.components) = rhs.mean_;

    covariance_.conservativeResize(NoChange, dim_covariance * new_components);
    covariance_.rightCols(dim_covariance * rhs.components) = rhs.covariance_;

    weight_.conservativeResize(new_components);
    weight_.tail(rhs.components) = rhs.weight_;

    return *this;
}


ParticleSet operator+(ParticleSet lhs, const ParticleSet& rhs)
{
    lhs += rhs;
    return lhs;
}


Ref<MatrixXd> ParticleSet::state()
{
    return state_;
}


Ref<MatrixXd> ParticleSet::state(const std::size_t i)
{
    return state_.col(i);
}


double& ParticleSet::state(const std::size_t i, const std::size_t j)
{
    return state_(j, i);
}


const Ref<const MatrixXd> ParticleSet::state() const
{
    return state_;
}


const Ref<const MatrixXd> ParticleSet::state(const std::size_t i) const
{
    return state_.col(i);
}


const double& ParticleSet::state(const std::size_t i, const std::size_t j) const
{
    return state_(j, i);
}
