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
    Gaussian();

    Gaussian(const std::size_t dim_linear);

    Gaussian(const std::size_t dim_linear, const std::size_t dim_circular);

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

    virtual ~Gaussian() noexcept { };
};

#endif /* GAUSSIAN_H */
