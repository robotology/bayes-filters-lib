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

    Gaussian(const unsigned int dim);

    virtual ~Gaussian() noexcept;

    Eigen::Ref<Eigen::VectorXd> mean;

    Eigen::Ref<Eigen::MatrixXd> covariance;

    double& weight;

    unsigned int dim;

protected:
    Gaussian(Eigen::Ref<Eigen::VectorXd> mean, Eigen::Ref<Eigen::MatrixXd> covariance, double& weight);

    friend class GaussianMixture;

private:
    bool used_public_ctor_;

};

#endif /* GAUSSIAN_H */
