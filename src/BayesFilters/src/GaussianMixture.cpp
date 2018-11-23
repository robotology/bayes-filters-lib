#include <BayesFilters/GaussianMixture.h>

using namespace bfl;
using namespace Eigen;


GaussianMixture::GaussianMixture() :
    GaussianMixture(1, 1, 0)
{ }


GaussianMixture::GaussianMixture(const std::size_t components, const std::size_t dim) :
    GaussianMixture(components, dim, 0)
{ }


GaussianMixture::GaussianMixture
(
    const std::size_t components,
    const std::size_t dim_linear,
    const std::size_t dim_circular
) :
    components(components),
    dim(dim_linear + dim_circular),
    dim_linear(dim_linear),
    dim_circular(dim_circular),
    mean_(dim, components),
    covariance_(dim, dim * components),
    weight_(components)
{
    for (int i = 0; i < this->components; ++i)
        weight_(i) = 1.0 / this->components;
}


GaussianMixture::~GaussianMixture() noexcept { }


Ref<MatrixXd> GaussianMixture::mean()
{
    return mean_;
}


Ref<VectorXd> GaussianMixture::mean(const std::size_t i)
{
    return mean_.col(i);
}


const Ref<const MatrixXd> GaussianMixture::mean() const
{
    return mean_;
}


const Ref<const VectorXd> GaussianMixture::mean(const std::size_t i) const
{
    return mean_.col(i);
}


Ref<MatrixXd> GaussianMixture::covariance()
{
    return covariance_;
}


Ref<MatrixXd> GaussianMixture::covariance(const std::size_t i)
{
    return covariance_.middleCols(this->dim * i, this->dim);
}


const Ref<const MatrixXd> GaussianMixture::covariance() const
{
    return covariance_;
}


const Ref<const MatrixXd> GaussianMixture::covariance(const std::size_t i) const
{
    return covariance_.middleCols(this->dim * i, this->dim);
}


Ref<VectorXd> GaussianMixture::weight()
{
    return weight_;
}


double& GaussianMixture::weight(const std::size_t i)
{
    return weight_(i);
}


const Ref<const VectorXd> GaussianMixture::weight() const
{
    return weight_;
}


const double& GaussianMixture::weight(const std::size_t i) const
{
    return weight_(i);
}
