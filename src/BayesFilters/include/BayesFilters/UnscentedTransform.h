#ifndef UNSCENTEDTRANSFORM_H
#define UNSCENTEDTRANSFORM_H

#include <cmath>

#include <Eigen/Dense>

namespace bfl
{

    void UnscentedTransform(const unsigned int n, const double alpha, const double beta, const double kappa,
                            Eigen::Ref<Eigen::VectorXd> weight_mean, Eigen::Ref<Eigen::VectorXd> weight_covariance, double& c);

}

#endif /* UNSCENTEDTRANSFORM_H */
