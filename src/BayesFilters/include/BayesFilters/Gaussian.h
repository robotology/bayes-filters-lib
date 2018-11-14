#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <memory>

#include <Eigen/Core>

namespace bfl {
    class Gaussian;
}


class bfl::Gaussian
{
public:
    Gaussian();

    Gaussian(const std::size_t dim);

    Gaussian(const std::size_t dim_linear, const std::size_t dim_circular);

    Gaussian(const Eigen::Ref<const Eigen::VectorXd>& mean, const Eigen::Ref<const Eigen::MatrixXd>& covariance, const double weight);

    Gaussian(const Eigen::Ref<const Eigen::VectorXd>& mean, const Eigen::Ref<const Eigen::MatrixXd>& covariance, const double weight,
             const std::size_t dim_linear, const std::size_t dim_circular);

    Gaussian(const Gaussian& gaussian);

    Gaussian(Gaussian&& gaussian);

    Gaussian& operator=(const Gaussian& gaussian);

    Gaussian& operator=(Gaussian&& gaussian);

    virtual ~Gaussian() noexcept;

    std::size_t dim;

    std::size_t dim_linear;

    std::size_t dim_circular;

    Eigen::VectorXd mean;

    Eigen::MatrixXd covariance;

    double weight;
};

#endif /* GAUSSIAN_H */
