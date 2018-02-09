#include "BayesFilters/Gaussian.h"

#include <exception>

using namespace bfl;
using namespace Eigen;


Gaussian::Gaussian() :
    Gaussian(1) { }


Gaussian::Gaussian(unsigned int dim) :
    mean(*(new VectorXd(dim))),
    covariance(*(new MatrixXd(dim, dim))),
    weight(*(new double{1.0})),
    dim(dim),
    used_public_ctor_(true)
{
    if (dim == 0)
        throw std::runtime_error("ERROR::GAUSSIAN::CTOR\nERROR:\n\tDimension cannot be 0.");
}


Gaussian::Gaussian(Ref<VectorXd> mean, Ref<MatrixXd> covariance, double& weight) :
    mean(mean),
    covariance(covariance),
    weight(weight),
    dim(mean.rows()),
    used_public_ctor_(false) { }


Gaussian::~Gaussian() noexcept
{
    if (used_public_ctor_)
    {
        mean.~Ref();
        covariance.~Ref();
        delete &weight;
    }
}
