#include <utility>

#include "BayesFilters/DrawParticle.h"

using namespace bfl;
using namespace Eigen;


DrawParticle::DrawParticle(std::unique_ptr<StateModel> transition_model) noexcept :
    state_model_(std::move(transition_model)) { }


DrawParticle::~DrawParticle() noexcept { }


DrawParticle::DrawParticle(DrawParticle&& pf_prediction) noexcept :
    state_model_(std::move(pf_prediction.state_model_)) { };


DrawParticle& DrawParticle::operator=(DrawParticle&& pf_prediction) noexcept
{
    state_model_ = std::move(pf_prediction.state_model_);

    return *this;
}


void DrawParticle::motion(const Ref<const VectorXf>& cur_state, Ref<VectorXf> prop_state)
{
    state_model_->propagate(cur_state, prop_state);
}


void DrawParticle::motionDisturbance(Ref<VectorXf> sample)
{
    state_model_->noiseSample(sample);
}


void DrawParticle::predict(const Ref<const VectorXf>& prev_state, Ref<VectorXf> pred_state)
{
    motion(prev_state, pred_state);

    VectorXf sample(VectorXf::Zero(prev_state.rows(), 1));
    motionDisturbance(sample);

    pred_state += sample;
}

bool DrawParticle::setStateModelProperty(const std::string& property)
{
    return state_model_->setProperty(property);
}
