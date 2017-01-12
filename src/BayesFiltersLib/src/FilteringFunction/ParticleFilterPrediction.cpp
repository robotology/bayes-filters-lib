#include <FilteringFunction/ParticleFilterPrediction.h>
#include <FilteringFunction/StateModel.h>

using namespace Eigen;


ParticleFilterPrediction::ParticleFilterPrediction(std::shared_ptr<StateModel> transition_model) noexcept :
    transition_model_(transition_model) { };


ParticleFilterPrediction::~ParticleFilterPrediction() noexcept { };


ParticleFilterPrediction::ParticleFilterPrediction(const ParticleFilterPrediction& pf_prediction)
{
    transition_model_ = pf_prediction.transition_model_;
};


ParticleFilterPrediction::ParticleFilterPrediction(ParticleFilterPrediction&& pf_prediction) noexcept :
    transition_model_(std::move(pf_prediction.transition_model_)) { };


ParticleFilterPrediction& ParticleFilterPrediction::operator=(const ParticleFilterPrediction& pf_prediction)
{
    ParticleFilterPrediction tmp(pf_prediction);
    *this = std::move(tmp);

    return *this;
};


ParticleFilterPrediction& ParticleFilterPrediction::operator=(ParticleFilterPrediction&& pf_prediction) noexcept
{
    transition_model_ = std::move(pf_prediction.transition_model_);
    
    return *this;
};


void ParticleFilterPrediction::predict(const Ref<const VectorXf> & prev_state, Ref<VectorXf> pred_state)
{
    transition_model_->propagate(prev_state, pred_state);

    Vector4f process_noise;
    transition_model_->noiseSample(process_noise);
    pred_state += process_noise;
}
