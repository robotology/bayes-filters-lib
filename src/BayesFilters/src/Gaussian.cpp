#include "BayesFilters/Gaussian.h"

using namespace bfl;
using namespace Eigen;


Gaussian::Gaussian(unsigned int dim) :
    mean(dim),
    covariance(dim, dim) { }


Gaussian::~Gaussian() { }
