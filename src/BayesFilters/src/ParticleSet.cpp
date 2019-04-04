/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/ParticleSet.h>

using namespace bfl;
using namespace Eigen;


ParticleSet::ParticleSet(const std::size_t components, const std::size_t dim) noexcept:
    ParticleSet(components, dim, 0) { }


ParticleSet::ParticleSet
(
    const std::size_t components,
    const std::size_t dim_linear,
    const std::size_t dim_circular
) noexcept :
    GaussianMixture(components, dim_linear, dim_circular),
    state_(dim, components) { }


ParticleSet::~ParticleSet() noexcept { }


ParticleSet& ParticleSet::operator+=(const ParticleSet& rhs)
{
    /* Should check whether (this->dim_linear == rhs.dim_linear) &&
       (this->dim_circular == rhs.dim_circular). */
    std::size_t new_components = components + rhs.components;

    state_.conservativeResize(NoChange,  new_components);
    state_.rightCols(rhs.components) = rhs.state_;

    mean_.conservativeResize(NoChange,  new_components);
    mean_.rightCols(rhs.components) = rhs.mean_;

    covariance_.conservativeResize(NoChange, dim * new_components);
    covariance_.rightCols(dim * rhs.components) = rhs.covariance_;

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
