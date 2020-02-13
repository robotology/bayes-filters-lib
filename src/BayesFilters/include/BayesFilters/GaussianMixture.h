/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef GAUSSIANMIXTURE_H
#define GAUSSIANMIXTURE_H

#include <vector>

#include <Eigen/Dense>

namespace bfl {
    class GaussianMixture;
}


class bfl::GaussianMixture
{
public:
    GaussianMixture() noexcept;

    GaussianMixture(const std::size_t components, const std::size_t dim) noexcept;

    GaussianMixture(const std::size_t components, const std::size_t dim_linear, const std::size_t dim_circular, const bool use_quaternion = false) noexcept;

    virtual ~GaussianMixture() noexcept;

    virtual void resize(const std::size_t components, const std::size_t dim_linear, const std::size_t dim_circular = 0);

    Eigen::Ref<Eigen::MatrixXd> mean();

    Eigen::Ref<Eigen::VectorXd> mean(const std::size_t i);

    double& mean(const std::size_t i, const std::size_t j);

    const Eigen::Ref<const Eigen::MatrixXd> mean() const;

    const Eigen::Ref<const Eigen::VectorXd> mean(const std::size_t i) const;

    const double& mean(const std::size_t i, const std::size_t j) const;

    Eigen::Ref<Eigen::MatrixXd> covariance();

    Eigen::Ref<Eigen::MatrixXd> covariance(const std::size_t i);

    double& covariance(const std::size_t i, const std::size_t j, const std::size_t k);

    const Eigen::Ref<const Eigen::MatrixXd> covariance() const;

    const Eigen::Ref<const Eigen::MatrixXd> covariance(const std::size_t i) const;

    const double& covariance(const std::size_t i, const std::size_t j, const std::size_t k) const;

    Eigen::Ref<Eigen::VectorXd> weight();

    double& weight(const std::size_t i);

    const Eigen::Ref<const Eigen::VectorXd> weight() const;

    const double& weight(const std::size_t i) const;

    bool augmentWithNoise(const Eigen::Ref<const Eigen::MatrixXd>& noise_covariance_matrix);

    std::size_t components;

    bool use_quaternion;

    std::size_t dim_circular_component;

    std::size_t dim;

    std::size_t dim_linear;

    std::size_t dim_circular;

    std::size_t dim_noise;

    std::size_t dim_covariance;

protected:
    Eigen::MatrixXd mean_;

    Eigen::MatrixXd covariance_;

    Eigen::VectorXd weight_;
};

#endif /* GAUSSIANMIXTURE_H */
