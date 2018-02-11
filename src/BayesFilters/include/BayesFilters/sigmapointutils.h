#ifndef SIGMAPOINTUTILS_H
#define SIGMAPOINTUTILS_H

#include <cmath>

#include <Eigen/Dense>

#include <BayesFilters/Gaussian.h>


namespace bfl
{
    void unscented_weights(const unsigned int n, const double alpha, const double beta, const double kappa,
                           Eigen::Ref<Eigen::VectorXd> weight_mean, Eigen::Ref<Eigen::VectorXd> weight_covariance, double& c);

    Eigen::MatrixXd unscented_transform(const Gaussian& state, const double c);
}

#endif /* SIGMAPOINTUTILS_H */
