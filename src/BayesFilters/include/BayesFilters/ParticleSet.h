/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef PARTICLESET_H
#define PARTICLESET_H

#include <BayesFilters/GaussianMixture.h>

#include <vector>

namespace bfl {
    class ParticleSet;
}


class bfl::ParticleSet : public bfl::GaussianMixture
{
public:
    ParticleSet() noexcept;

    ParticleSet(const std::size_t components, const std::size_t dim) noexcept;

    ParticleSet(const std::size_t components, const std::size_t dim_linear, const std::size_t dim_circular, const bool use_quaternion = false) noexcept;

    virtual ~ParticleSet() noexcept = default;

    void resize(const std::size_t components, const std::size_t dim_linear, const std::size_t dim_circular = 0) override;

    ParticleSet& operator+=(const ParticleSet& rhs);

    Eigen::Ref<Eigen::MatrixXd> state();

    Eigen::Ref<Eigen::MatrixXd> state(const std::size_t i);

    double& state(const std::size_t i, const std::size_t j);

    const Eigen::Ref<const Eigen::MatrixXd> state() const;

    const Eigen::Ref<const Eigen::MatrixXd> state(const std::size_t i) const;

    const double& state(const std::size_t i, const std::size_t j) const;


protected:
    Eigen::MatrixXd state_;
};


bfl::ParticleSet operator+(bfl::ParticleSet lhs, const bfl::ParticleSet& rhs);

#endif /* PARTICLESET_H */
