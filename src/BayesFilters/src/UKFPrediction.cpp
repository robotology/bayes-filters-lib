#include "BayesFilters/UKFPrediction.h"

#include <utility>

using namespace bfl;
using namespace Eigen;


UKFPrediction::UKFPrediction() noexcept { }


UKFPrediction::UKFPrediction(UKFPrediction&& ukf_prediction) noexcept :
    SPPrediction(std::move(ukf_prediction)) { };


UKFPrediction::~UKFPrediction() noexcept { }


Gaussian UKFPrediction::predictStep(const Gaussian& prev_states)
{
}

StateModel& UKFPrediction::getStateModel()
{
    return *state_model_;
}


void UKFPrediction::setStateModel(std::unique_ptr<StateModel> state_model)
{
    state_model_ = std::move(state_model);
}
