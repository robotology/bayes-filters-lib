#include <BayesFilters/GaussianRef.h>

#include <exception>

using namespace bfl;
using namespace Eigen;


GaussianRef::GaussianRef
(
    Ref<VectorXd> mean,
    Ref<MatrixXd> covariance,
    double& weight
) :
    mean(mean),
    covariance(covariance),
    weight(weight)
{ }


GaussianRef::GaussianRef(const GaussianRef& gaussian) :
    mean(gaussian.mean),
    covariance(gaussian.covariance),
    weight(gaussian.weight)
{ }


GaussianRef::GaussianRef(GaussianRef&& gaussian) :
    mean(std::move(gaussian.mean)),
    covariance(std::move(gaussian.covariance)),
    weight(gaussian.weight)
{ }


GaussianConstRef::GaussianConstRef
(
    const Ref<const VectorXd> mean,
    const Ref<const MatrixXd> covariance,
    const double& weight
) :
    mean(mean),
    covariance(covariance),
    weight(weight)
{ }


GaussianConstRef::GaussianConstRef(const GaussianConstRef& gaussian) :
    mean(gaussian.mean),
    covariance(gaussian.covariance),
    weight(gaussian.weight)
{ }


GaussianConstRef::GaussianConstRef(GaussianConstRef&& gaussian) :
    mean(std::move(gaussian.mean)),
    covariance(std::move(gaussian.covariance)),
    weight(gaussian.weight)
{ }