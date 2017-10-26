#include "BayesFilters/PFPrediction.h"

using namespace bfl;


PFPrediction::PFPrediction() noexcept { };


PFPrediction::PFPrediction(PFPrediction&& pf_prediction) noexcept :
    state_model_(std::move(pf_prediction.state_model_)) { }


void PFPrediction::predict(const Eigen::Ref<const Eigen::MatrixXf>& prev_states, const Eigen::Ref<const Eigen::VectorXf>& prev_weights,
                           Eigen::Ref<Eigen::MatrixXf> pred_states, Eigen::Ref<Eigen::VectorXf> pred_weights)
{
    if (!skip_)
        predictStep(prev_states, prev_weights,
                    pred_states, pred_weights);
    else
    {
        pred_states  = prev_states;
        pred_weights = prev_states;
    }
}


bool PFPrediction::skip(const bool status)
{
    skip_ = status;

    return true;
}


StateModel& PFPrediction::getStateModel()
{
    return *state_model_;
}


void PFPrediction::setStateModel(std::unique_ptr<StateModel> state_model)
{
    state_model_ = std::move(state_model);
}
