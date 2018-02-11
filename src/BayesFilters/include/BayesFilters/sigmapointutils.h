#ifndef SIGMAPOINTUTILS_H
#define SIGMAPOINTUTILS_H

#include <cmath>

#include <Eigen/Dense>

#include <BayesFilters/Gaussian.h>
#include <BayesFilters/GaussianMixture.h>


namespace bfl
{
    void UnscentedWeights(const unsigned int n, const double alpha, const double beta, const double kappa,
                          Eigen::Ref<Eigen::VectorXd> weight_mean, Eigen::Ref<Eigen::VectorXd> weight_covariance, double& c);

    GaussianMixture UnscentedTransform(const Gaussian& state, const double c);
}

#endif /* SIGMAPOINTUTILS_H */
