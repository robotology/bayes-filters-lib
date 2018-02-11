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
    Gaussian() noexcept;

    Gaussian(const unsigned int dim);

    Gaussian(const unsigned int dim_linear, const unsigned int dim_circular);

    virtual ~Gaussian() noexcept;

    unsigned int dim;

    unsigned int dim_linear;

    unsigned int dim_circular;

    Eigen::Ref<Eigen::VectorXd> mean;

    Eigen::Ref<Eigen::MatrixXd> covariance;

    double& weight;

protected:
    Gaussian(Eigen::Ref<Eigen::VectorXd> mean, Eigen::Ref<Eigen::MatrixXd> covariance, double& weight);

    Gaussian(Eigen::Ref<Eigen::VectorXd> mean, Eigen::Ref<Eigen::MatrixXd> covariance, double& weight,
             const unsigned int dim_linear, const unsigned int dim_circular);

    friend class GaussianMixture;

private:
    bool used_public_ctor_;

};

#endif /* GAUSSIAN_H */
