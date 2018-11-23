#include <BayesFilters/GaussianCorrection.h>

using namespace bfl;
using namespace Eigen;


GaussianCorrection::GaussianCorrection() noexcept { };


void GaussianCorrection::correct(const GaussianMixture& pred_state, GaussianMixture& corr_state)
{
    /* Perform correction if required and if measurements can be frozen. */    
    if((!skip_) && getMeasurementModel().freezeMeasurements())
        correctStep(pred_state, corr_state);
    else
        corr_state = pred_state;
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
