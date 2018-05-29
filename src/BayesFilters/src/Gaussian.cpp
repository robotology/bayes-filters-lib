#include <BayesFilters/Gaussian.h>

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


Gaussian::Gaussian(const Gaussian& gaussian) :
    dim(gaussian.dim),
    dim_linear(gaussian.dim_linear),
    dim_circular(gaussian.dim_circular),
    mean(*(new VectorXd(dim))),
    covariance(*(new MatrixXd(dim, dim))),
    weight(*(new double{gaussian.weight})),
    used_public_ctor_(true)
{
    mean << gaussian.mean;
    covariance << gaussian.covariance;
}


Gaussian::Gaussian(Gaussian&& gaussian) :
    dim(gaussian.dim),
    dim_linear(gaussian.dim_linear),
    dim_circular(gaussian.dim_circular),
    mean(*(new VectorXd(dim))),
    covariance(*(new MatrixXd(dim, dim))),
    weight(*(new double{gaussian.weight})),
    used_public_ctor_(true)
{
    mean << gaussian.mean;
    covariance << gaussian.covariance;

    gaussian.mean.Zero(gaussian.dim);

    gaussian.covariance.Zero(gaussian.dim, gaussian.dim);

    gaussian.weight = 1.0;

    gaussian.dim = 0;

    gaussian.dim_linear = 0;

    gaussian.dim_circular = 0;
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


Gaussian& Gaussian::operator=(const Gaussian& gaussian)
{
    if (&gaussian != this)
    {
        Gaussian tmp(gaussian);
        *this = std::move(tmp);
    }

    return *this;
}


Gaussian& Gaussian::operator=(Gaussian&& gaussian)
{
    if (&gaussian != this)
    {
        mean = std::move(gaussian.mean);
        gaussian.mean.Zero(gaussian.dim);

        covariance = std::move(gaussian.covariance);
        gaussian.covariance.Zero(gaussian.dim, gaussian.dim);

        weight = gaussian.weight;
        gaussian.weight = 1.0;

        dim = gaussian.dim;
        gaussian.dim = 0;

        dim_linear = gaussian.dim_linear;
        gaussian.dim_linear = 0;

        dim_circular = gaussian.dim_circular;
        gaussian.dim_circular = 0;

        used_public_ctor_ = gaussian.used_public_ctor_;
    }

    return *this;
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
