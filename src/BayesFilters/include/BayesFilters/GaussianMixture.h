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
    GaussianMixture();

    GaussianMixture(const std::size_t components, const std::size_t dim);

    GaussianMixture(const std::size_t components, const std::size_t dim_linear, const std::size_t dim_circular);

    virtual ~GaussianMixture() noexcept;

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

    std::size_t components;

    std::size_t dim;

    std::size_t dim_linear;

    std::size_t dim_circular;

    std::size_t dim_noise;

protected:
    Eigen::MatrixXd mean_;

    Eigen::MatrixXd covariance_;

    Eigen::VectorXd weight_;
};

#endif /* GAUSSIANMIXTURE_H */
