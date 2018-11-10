#ifndef SIGMAPOINT_H
#define SIGMAPOINT_H

#include <BayesFilters/Gaussian.h>

#include <cmath>

#include <Eigen/Dense>


namespace bfl
{
namespace sigma_point
{

    void unscented_weights(const unsigned int n, const double alpha, const double beta, const double kappa,
                           Eigen::Ref<Eigen::VectorXd> weight_mean, Eigen::Ref<Eigen::VectorXd> weight_covariance, double& c);

    Eigen::MatrixXd sigma_point(const Gaussian& state, const double c);

}
}

#endif /* SIGMAPOINT_H */
