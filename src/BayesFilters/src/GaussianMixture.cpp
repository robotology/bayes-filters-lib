#include <BayesFilters/GaussianMixture.h>

using namespace bfl;
using namespace Eigen;


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
    mean(dim, components),
    covariance(dim, dim * components),
    weight(components)
{
    for (int i = 0; i < this->components; ++i)
        weight(i) = 1.0 / this->components;
}


GaussianMixture::~GaussianMixture() noexcept { }


GaussianRef GaussianMixture::operator[](const std::size_t i)
{
    return GaussianRef(mean.col(i), covariance.middleCols(this->dim * i, this->dim), weight(i));
}


const GaussianConstRef GaussianMixture::operator[](const std::size_t i) const
{
    return GaussianConstRef(mean.col(i), covariance.middleCols(this->dim * i, this->dim), weight(i));
}
