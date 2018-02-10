#include "BayesFilters/GaussianMixture.h"

using namespace bfl;
using namespace Eigen;


GaussianMixture::GaussianMixture(const unsigned int components, const unsigned int dim) :
    means(dim, components),
    covariances(dim, dim * components),
    weights(components),
    components(components),
    dim(dim)
{
    for (int i = 0; i < this->components; ++i)
    {
        weights(i) = 1.0 / this->components;
        gaussian_.emplace_back(Gaussian(means.col(i), covariances.middleCols(dim * i, dim), weights(i)));
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
