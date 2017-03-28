#include <utility>

#include "BayesFiltersLib/ParticleFilterPrediction.h"
#include "BayesFiltersLib/StateModel.h"

using namespace Eigen;


namespace bfl
{

ParticleFilterPrediction::ParticleFilterPrediction(std::unique_ptr<StateModel> transition_model) noexcept :
    transition_model_(std::move(transition_model)) { }


ParticleFilterPrediction::~ParticleFilterPrediction() noexcept { }


ParticleFilterPrediction::ParticleFilterPrediction(ParticleFilterPrediction&& pf_prediction) noexcept :
    transition_model_(std::move(pf_prediction.transition_model_)) { };


ParticleFilterPrediction& ParticleFilterPrediction::operator=(ParticleFilterPrediction&& pf_prediction) noexcept
{
    transition_model_ = std::move(pf_prediction.transition_model_);

    return *this;
}


void ParticleFilterPrediction::predict(const Ref<const VectorXf>& prev_state, Ref<VectorXf> pred_state)
{
    transition_model_->propagate(prev_state, pred_state);

    VectorXf sample(VectorXf::Zero(prev_state.rows(), 1));
    transition_model_->noiseSample(sample);

    pred_state += sample;
}

} // namespace bfl
