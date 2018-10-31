#include <BayesFilters/SPCorrection.h>

using namespace bfl;
using namespace Eigen;


SPCorrection::SPCorrection() noexcept { };


Gaussian SPCorrection::correct(const Gaussian& pred_state)
{
    if (!skip_)
        return correctStep(pred_state);
    else
        return pred_state;
}


std::pair<bool, VectorXd> SPCorrection::getLikelihood()
{
    return std::make_pair(false, VectorXd::Zero(1));
}


bool SPCorrection::skip(const bool status)
{
    skip_ = status;

    return true;
}


void SPCorrection::setMeasurementModel(std::unique_ptr<MeasurementModel> measurement_model)
{
    measurement_model_ = std::move(measurement_model);
}
