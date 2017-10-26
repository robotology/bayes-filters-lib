#include "BayesFilters/PFPrediction.h"

using namespace bfl;
using namespace Eigen;


PFPrediction::PFPrediction() noexcept { };


PFPrediction::PFPrediction(PFPrediction&& pf_prediction) noexcept :
    state_model_(std::move(pf_prediction.state_model_)) { }


void PFPrediction::predict(const Ref<const MatrixXf>& prev_states, const Ref<const VectorXf>& prev_weights,
                           Ref<MatrixXf> pred_states, Ref<VectorXf> pred_weights)
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
