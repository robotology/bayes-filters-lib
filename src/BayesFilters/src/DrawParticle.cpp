#include <utility>

#include "BayesFilters/DrawParticle.h"

using namespace bfl;
using namespace Eigen;


DrawParticle::DrawParticle(std::unique_ptr<StateModel> transition_model) noexcept :
    state_model_(std::move(transition_model)) { }


DrawParticle::DrawParticle(DrawParticle&& pf_prediction) noexcept :
state_model_(std::move(pf_prediction.state_model_)) { };


DrawParticle::~DrawParticle() noexcept { }


DrawParticle& DrawParticle::operator=(DrawParticle&& pf_prediction) noexcept
{
    state_model_ = std::move(pf_prediction.state_model_);

    return *this;
}


void DrawParticle::predict(const Ref<const VectorXf>& prev_state, Ref<VectorXf> pred_state)
{
    state_model_->motion(prev_state, pred_state);
}


StateModel DrawParticle::getStateModel()
{
    return *state_model_;
}
