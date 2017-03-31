#include <utility>

#include "BayesFiltersLib/ParticleFilterPrediction.h"
#include "BayesFiltersLib/StateModel.h"

using namespace Eigen;


namespace bfl
{

ParticleFilterPrediction::ParticleFilterPrediction(std::unique_ptr<StateModel> transition_model) noexcept :
    state_model_(std::move(transition_model)) { }


ParticleFilterPrediction::~ParticleFilterPrediction() noexcept { }


ParticleFilterPrediction::ParticleFilterPrediction(ParticleFilterPrediction&& pf_prediction) noexcept :
    state_model_(std::move(pf_prediction.state_model_)) { };


ParticleFilterPrediction& ParticleFilterPrediction::operator=(ParticleFilterPrediction&& pf_prediction) noexcept
{
    state_model_ = std::move(pf_prediction.state_model_);

    return *this;
}


void ParticleFilterPrediction::predict(const Ref<const VectorXf>& prev_state, Ref<VectorXf> pred_state)
{
    state_model_->propagate(prev_state, pred_state);

    VectorXf sample(VectorXf::Zero(prev_state.rows(), 1));
    state_model_->noiseSample(sample);

    pred_state += sample;
}

bool ParticleFilterPrediction::setMotionModelProperty(const std::string& property)
{
    return state_model_->setProperty(property);
}

} // namespace bfl
