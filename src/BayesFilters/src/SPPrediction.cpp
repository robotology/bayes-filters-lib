#include <BayesFilters/SPPrediction.h>

#include <exception>
#include <iostream>

using namespace bfl;
using namespace Eigen;


SPPrediction::SPPrediction() noexcept { };


SPPrediction::SPPrediction(SPPrediction&& sp_prediction) noexcept { }


Gaussian SPPrediction::predict(const Gaussian& prev_state)
{
    if (!skip_prediction_)
        return predictStep(prev_state);
    else
        return prev_state;
}


bool SPPrediction::skip(const std::string& what_step, const bool status)
{
    if (what_step == "prediction")
        skip_prediction_ = status;
    else if (what_step == "state")
        skip_state_ = status;
    else if (what_step == "exogenous")
        skip_exogenous_ = status;
    else
        return false;

    return true;
}


bool SPPrediction::getSkipState()
{
    return skip_state_;
}


bool SPPrediction::getSkipExogenous()
{
    return skip_exogenous_;
}


ExogenousModel& SPPrediction::getExogenousModel()
{
    throw std::runtime_error("ERROR::PFPREDICTION::GETEXOGENOUSMODEL\nERROR:\n\tCall to unimplemented base class method.");
}


void SPPrediction::setExogenousModel(std::unique_ptr<ExogenousModel> exogenous_model)
{
    std::cerr << "ERROR::SPPREDICTION::SETEXOGENOUSMODEL\n";
    std::cerr << "ERROR:\n\tCall to unimplemented base class method." << std::endl;
}
