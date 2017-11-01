#include "BayesFilters/PFPrediction.h"

#include <exception>
#include <iostream>

using namespace bfl;
using namespace Eigen;


PFPrediction::PFPrediction() noexcept { };


PFPrediction::PFPrediction(PFPrediction&& pf_prediction) noexcept { }


void PFPrediction::predict(const Ref<const MatrixXf>& prev_states, const Ref<const VectorXf>& prev_weights,
                           Ref<MatrixXf> pred_states, Ref<VectorXf> pred_weights)
{
    if (!skip_prediction_)
        predictStep(prev_states, prev_weights,
                    pred_states, pred_weights);
    else
    {
        pred_states  = prev_states;
        pred_weights = prev_states;
    }
}


bool PFPrediction::skip(const std::string& what_step, const bool status)
{
    if (what_step == "prediction")
        skip_prediction_ = status;
    else if (what_step == "pred_state")
        skip_state_ = status;
    else if (what_step == "pred_exogenous")
        skip_exogenous_ = status;
    else
        return false;

    return true;
}


bool PFPrediction::getSkipState()
{
    return skip_state_;
}


bool PFPrediction::getSkipExogenous()
{
    return skip_exogenous_;
}


ExogenousModel& PFPrediction::getExogenousModel()
{
    throw std::runtime_error("ERROR::PFPREDICTION::GETEXOGENOUSMODEL\nERROR:\n\tCall to unimplemented base class method.");
}


void PFPrediction::setExogenousModel(std::unique_ptr<ExogenousModel> exogenous_model)
{
    std::cerr << "ERROR::PFPREDICTION::SETEXOGENOUSMODEL" << std::endl;
    std::cerr << "ERROR:\n\tCall to unimplemented base class method.";
}
