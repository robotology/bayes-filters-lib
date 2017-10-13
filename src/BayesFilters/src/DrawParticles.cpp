#include "BayesFilters/DrawParticles.h"

#include <utility>

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


void DrawParticles::predict(const Ref<const MatrixXf>& prev_states, const Ref<const VectorXf>& prev_weights,
                            Ref<MatrixXf> pred_states, Ref<VectorXf> pred_weights)
{
    state_model_->motion(prev_states, pred_states);
    
    pred_weights = prev_weights;
}


StateModel& DrawParticles::getStateModel()
{
    return *state_model_;
}
