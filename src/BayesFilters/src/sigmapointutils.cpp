#include "BayesFilters/sigmapointutils.h"
#include "BayesFilters/directionalstatisticsutils.h"

#include <Eigen/SVD>

using namespace bfl;
using namespace Eigen;


void bfl::UnscentedWeights(const unsigned int n, const double alpha, const double beta, const double kappa,
                           Ref<VectorXd> weight_mean, Ref<VectorXd> weight_covariance, double& c)
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


GaussianMixture bfl::UnscentedTransform(const Gaussian& state, const double c)
{
    JacobiSVD<MatrixXd> svd = state.covariance.jacobiSvd(ComputeFullU);

    MatrixXd A = svd.matrixU() * svd.singularValues().cwiseSqrt().asDiagonal();

    GaussianMixture sigma_points((state.dim * 2) + 1, state.dim_linear, state.dim_circular);

    sigma_points.means << VectorXd::Zero(state.dim), std::sqrt(c) * A, -std::sqrt(c) * A;

    if (sigma_points.dim_linear > 0)
        sigma_points.means.topRows(sigma_points.dim_linear).colwise() += state.mean.topRows(state.dim_linear);

    if (sigma_points.dim_circular > 0)
        sigma_points.means.bottomRows(sigma_points.dim_circular) = directional_add(sigma_points.means.bottomRows(sigma_points.dim_circular), state.mean.bottomRows(state.dim_circular));

    return sigma_points;
}
