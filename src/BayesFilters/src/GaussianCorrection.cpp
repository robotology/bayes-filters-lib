#include <BayesFilters/GaussianCorrection.h>

using namespace bfl;
using namespace Eigen;


GaussianCorrection::GaussianCorrection() noexcept { };


Gaussian GaussianCorrection::correct(const Gaussian& pred_state)
{
    if (!skip_)
        return correctStep(pred_state);
    else
        return pred_state;
}


std::pair<bool, VectorXd> GaussianCorrection::getLikelihood()
{
    return std::make_pair(false, VectorXd::Zero(1));
}


bool GaussianCorrection::skip(const bool status)
{
    skip_ = status;

    return true;
}
