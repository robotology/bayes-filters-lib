#include "BayesFilters/UnscentedTransform.h"


void bfl::UnscentedTransform(const unsigned int n, const double alpha, const double beta, const double kappa,
                             Eigen::Ref<Eigen::VectorXd> weight_mean, Eigen::Ref<Eigen::VectorXd> weight_covariance, double& c)
{
    double lambda = std::pow(alpha, 2.0) * (n + kappa) - n;
    
    for (int j = 0; j < ((2 * n) + 1); ++j)
    {
        if (j == 0)
        {
            weight_mean(j)       = lambda / (n + lambda);
            weight_covariance(j) = lambda / (n + lambda) + (1 - std::pow(alpha, 2.0) + beta);
        }
        else
        {
            weight_mean(j)       = 1 / (2 * (n + lambda));
            weight_covariance(j) = weight_mean(j);
        }
    }
    
    c = n + lambda;
}
