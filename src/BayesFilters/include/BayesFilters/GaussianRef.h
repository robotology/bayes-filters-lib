#ifndef GAUSSIANREF_H
#define GAUSSIANREF_H

#include <memory>

#include <Eigen/Core>

namespace bfl {
    class GaussianRef;
    class GaussianConstRef;
}


class bfl::GaussianRef
{
public:
    GaussianRef(const GaussianRef& gaussian_ref);

    GaussianRef(GaussianRef&& gaussian_ref);

    Eigen::Ref<Eigen::VectorXd> mean;

    Eigen::Ref<Eigen::MatrixXd> covariance;

    double& weight;

protected:
    GaussianRef(Eigen::Ref<Eigen::VectorXd> mean, Eigen::Ref<Eigen::MatrixXd> covariance, double& weight);

    friend class GaussianMixture;
};


class bfl::GaussianConstRef
{
public:
    GaussianConstRef(const GaussianConstRef& gaussian_ref);

    GaussianConstRef(GaussianConstRef&& gaussian_ref);

    const Eigen::Ref<const Eigen::VectorXd> mean;

    const Eigen::Ref<const Eigen::MatrixXd> covariance;

    const double& weight;

protected:
    GaussianConstRef(const Eigen::Ref<const Eigen::VectorXd> mean, const Eigen::Ref<const Eigen::MatrixXd> covariance, const double& weight);

    friend class GaussianMixture;
};

#endif /* GAUSSIANREF_H */
