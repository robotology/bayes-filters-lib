#include "BayesFilters/GaussianMixture.h"

using namespace bfl;
using namespace Eigen;


GaussianMixture::GaussianMixture(const unsigned int components, const unsigned int dim) :
    components(components),
    dim(dim),
    dim_linear(dim),
    dim_circular(0),
    means(dim, components),
    covariances(dim, dim * components),
    weights(components)
{
    for (int i = 0; i < this->components; ++i)
    {
        weights(i) = 1.0 / this->components;
        gaussian_.emplace_back(Gaussian(means.col(i), covariances.middleCols(this->dim * i, this->dim), weights(i)));
    }
}


GaussianMixture::GaussianMixture(const unsigned int components, const unsigned int dim_linear, const unsigned int dim_circular) :
    components(components),
    dim(dim_linear + dim_circular),
    dim_linear(dim_linear),
    dim_circular(dim_circular),
    means(dim, components),
    covariances(dim, dim * components),
    weights(components)
{
    for (int i = 0; i < this->components; ++i)
    {
        weights(i) = 1.0 / this->components;
        gaussian_.emplace_back(Gaussian(means.col(i), covariances.middleCols(this->dim * i, this->dim), weights(i),
                                        this->dim_linear, this->dim_circular));
    }
}


GaussianMixture::~GaussianMixture() noexcept { }


Gaussian& GaussianMixture::operator[](unsigned int i)
{
    return gaussian_[i];
}


const Gaussian& GaussianMixture::operator[](unsigned int i) const
{
    return gaussian_[i];
}
