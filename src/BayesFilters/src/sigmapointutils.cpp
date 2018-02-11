#include "BayesFilters/sigmapointutils.h"

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
//    [U, S] = svd(P);
//    A = U * sqrt(S);
//    X = [zeros(size(M)) A -A];
//    X = sqrt(c)*X + repmat(M, 1, size(X, 2));

    JacobiSVD<MatrixXd> svd = state.covariance.jacobiSvd(ComputeFullU);

    MatrixXd A = svd.matrixU() * svd.singularValues().cwiseSqrt().asDiagonal();

    GaussianMixture sigma_points((state.dim * 2) + 1, state.dim);

    sigma_points.means << VectorXd::Zero(state.dim), std::sqrt(c) * A, -std::sqrt(c) * A;

    sigma_points.means.colwise() += state.mean;

    return sigma_points;
}
