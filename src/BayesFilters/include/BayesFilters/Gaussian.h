/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <BayesFilters/GaussianMixture.h>

#include <memory>

#include <Eigen/Core>

namespace bfl {
    class Gaussian;
}


class bfl::Gaussian : public bfl::GaussianMixture
{
public:
    Gaussian() noexcept;

    Gaussian(const std::size_t dim_linear) noexcept;

    Gaussian(const std::size_t dim_linear, const std::size_t dim_circular, const bool use_quaternion = false) noexcept;

    virtual ~Gaussian() noexcept = default;

    void resize(const std::size_t dim_linear, const std::size_t dim_circular = 0);

    /**
     * Non-virtual methods of GaussianMixture are overriden here
     * since a Gaussian is a 1-component GaussianMixture.
     * Hence it is better to return a Ref<VectorXd> as the mean,
     * rather than a Ref<MatrixXd>, and a double& as the weight,
     * rather than a Ref<VectorXd>&.
     */
    Eigen::Ref<Eigen::VectorXd> mean();

    const Eigen::Ref<const Eigen::VectorXd> mean() const;

    double& mean(const std::size_t i);

    const double& mean(const std::size_t i) const;

    Eigen::Ref<Eigen::MatrixXd> covariance();

    const Eigen::Ref<const Eigen::MatrixXd> covariance() const;

    double& covariance(const std::size_t i, const std::size_t j);

    const double& covariance(const std::size_t i, const std::size_t j) const;

    double& weight();

    const double& weight() const;
};

#endif /* GAUSSIAN_H */
