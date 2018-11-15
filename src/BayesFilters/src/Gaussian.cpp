#include <BayesFilters/Gaussian.h>

#include <exception>

using namespace bfl;
using namespace Eigen;


Gaussian::Gaussian() :
    Gaussian(1, 0) { }


Gaussian::Gaussian(const std::size_t dim) :
    Gaussian(dim, 0) { }


Gaussian::Gaussian(const std::size_t dim_linear, const std::size_t dim_circular) :
    dim(dim_linear + dim_circular),
    dim_linear(dim_linear),
    dim_circular(dim_circular),
    mean(dim),
    covariance(dim, dim),
    weight{1.0}
{
    if (dim == 0)
        throw std::runtime_error("ERROR::GAUSSIAN::CTOR\nERROR:\n\tdim_linear + dim_circular cannot be 0.");
}


Gaussian::Gaussian(const Gaussian& gaussian) :
    dim(gaussian.dim),
    dim_linear(gaussian.dim_linear),
    dim_circular(gaussian.dim_circular),
    mean(gaussian.mean),
    covariance(gaussian.covariance),
    weight{ gaussian.weight}
{ }


Gaussian::Gaussian(Gaussian&& gaussian) :
    dim(gaussian.dim),
    dim_linear(gaussian.dim_linear),
    dim_circular(gaussian.dim_circular),
    mean(std::move(gaussian.mean)),
    covariance(std::move(gaussian.covariance)),
    weight{gaussian.weight}
{
    gaussian.mean.Zero(gaussian.dim);

    gaussian.covariance.Zero(gaussian.dim, gaussian.dim);

    gaussian.weight = 1.0;

    gaussian.dim = 0;

    gaussian.dim_linear = 0;

    gaussian.dim_circular = 0;
}


Gaussian::Gaussian
(
    const Ref<const VectorXd>& mean,
    const Ref<const MatrixXd>& covariance,
    double weight
) :
    Gaussian(mean, covariance, weight, mean.rows(), 0)
{ }


Gaussian::Gaussian
(
    const Ref<const VectorXd>& mean,
    const Ref<const MatrixXd>& covariance,
    const double weight,
    const std::size_t dim_linear,
    const std::size_t dim_circular
) :
    dim(dim_linear + dim_circular),
    dim_linear(dim_linear),
    dim_circular(dim_circular),
    mean(mean),
    covariance(covariance),
    weight(weight)
{
    if (dim == 0)
        throw std::runtime_error("ERROR::GAUSSIAN::CTOR\nERROR:\n\tdim_linear + dim_circular cannot be 0.");

    if (dim != mean.rows())
        throw std::runtime_error("ERROR::GAUSSIAN::CTOR\nERROR:\n\tdim_linear + dim_circular cannot be different from mean/covariance row dimension.");
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
    }

    return *this;
}


Gaussian::~Gaussian() noexcept
{ }
