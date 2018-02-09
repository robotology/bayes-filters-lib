#include "BayesFilters/GaussianMixture.h"

using namespace bfl;
using namespace Eigen;


GaussianMixture::GaussianMixture(unsigned int components, unsigned int dim) :
    components(components),
    dim(dim),
    mean(dim, components),
    covariance(components, MatrixXd(dim, dim)),
    weight(components)
{
    for (int i = 0; i < this->components; ++i)
    {
        weight(i) = 1.0 / this->components;
        gaussian_.emplace_back(Gaussian(mean.col(i), covariance[i], weight(i)));
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
