#include <utility>

#include "BayesFilters/DrawParticles.h"

using namespace bfl;
using namespace Eigen;


DrawParticles::DrawParticles(std::unique_ptr<StateModel> state_model) noexcept :
    state_model_(std::move(state_model)) { }


DrawParticles::DrawParticles(DrawParticles&& pf_prediction) noexcept :
    state_model_(std::move(pf_prediction.state_model_)) { };


DrawParticles::~DrawParticles() noexcept { }


DrawParticles& DrawParticles::operator=(DrawParticles&& pf_prediction) noexcept
{
    state_model_ = std::move(pf_prediction.state_model_);

    return *this;
}


void DrawParticles::predict(const Ref<const VectorXf>& prev_state, Ref<VectorXf> pred_state)
{
    state_model_->motion(prev_state, pred_state);
}


StateModel DrawParticles::getStateModel()
{
    return *state_model_;
}
