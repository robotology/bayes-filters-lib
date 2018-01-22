#include "BayesFilters/GaussianMixture.h"

using namespace bfl;
using namespace Eigen;


GaussianMixture::GaussianMixture(unsigned int num_component, unsigned int dim) :
    gaussian_(num_component, Gaussian(dim)) { }


GaussianMixture::~GaussianMixture() { }


Gaussian& GaussianMixture::operator[](unsigned int i)
{
    return gaussian_[i];
}


const Gaussian& GaussianMixture::operator[](unsigned int i) const
{
    return gaussian_[i];
}
