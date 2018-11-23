#include <BayesFilters/Gaussian.h>

#include <exception>

using namespace bfl;
using namespace Eigen;

Gaussian::Gaussian()
    : GaussianMixture(1, 1) { }

Gaussian::Gaussian(const std::size_t dim_linear)
    : GaussianMixture(1, dim_linear) { }


Gaussian::Gaussian(const std::size_t dim_linear, const std::size_t dim_circular)
    : GaussianMixture(1, dim_linear, dim_circular) { }


Ref<VectorXd> Gaussian::mean()
{
    return mean_.col(0);
}


const Ref<const VectorXd> Gaussian::mean() const
{
    return mean_.col(0);
}


double& Gaussian::weight()
{
    return weight_(0);
}


const double& Gaussian::weight() const
{
    return weight_(0);
}
