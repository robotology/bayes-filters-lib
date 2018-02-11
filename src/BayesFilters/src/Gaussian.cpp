#include "BayesFilters/Gaussian.h"

#include <exception>

using namespace bfl;
using namespace Eigen;


Gaussian::Gaussian() noexcept :
    Gaussian(1, 0) { }


Gaussian::Gaussian(const unsigned int dim) :
    Gaussian(dim, 0) { }


Gaussian::Gaussian(const unsigned int dim_linear, const unsigned int dim_circular) :
    dim(dim_linear + dim_circular),
    dim_linear(dim_linear),
    dim_circular(dim_circular),
    mean(*(new VectorXd(dim))),
    covariance(*(new MatrixXd(dim, dim))),
    weight(*(new double{1.0})),
    used_public_ctor_(true)
{
    if (dim == 0)
        throw std::runtime_error("ERROR::GAUSSIAN::CTOR\nERROR:\n\tDimension cannot be 0.");
}


Gaussian::Gaussian(Ref<VectorXd> mean, Ref<MatrixXd> covariance, double& weight) :
    Gaussian(mean, covariance, weight, mean.rows(), 0) { }


Gaussian::Gaussian(Ref<VectorXd> mean, Ref<MatrixXd> covariance, double& weight,
                   const unsigned int dim_linear, const unsigned int dim_circular) :
    dim(dim_linear + dim_circular),
    dim_linear(dim_linear),
    dim_circular(dim_circular),
    mean(mean),
    covariance(covariance),
    weight(weight),
    used_public_ctor_(false)
{
    if (dim == 0)
        throw std::runtime_error("ERROR::GAUSSIAN::CTOR\nERROR:\n\tDimension cannot be 0.");

    if (dim != mean.rows())
        throw std::runtime_error("ERROR::GAUSSIAN::CTOR\nERROR:\n\tDimension cannot be different from mean/covariance dimension.");
}


Gaussian::~Gaussian() noexcept
{
    if (used_public_ctor_)
    {
        mean.~Ref();
        covariance.~Ref();
        delete &weight;
    }
}
