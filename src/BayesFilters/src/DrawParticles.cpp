#include "BayesFilters/DrawParticles.h"

#include <utility>

using namespace bfl;
using namespace Eigen;


DrawParticles::DrawParticles() noexcept { }


DrawParticles::DrawParticles(DrawParticles&& pf_prediction) noexcept :
    PFPrediction(std::move(pf_prediction)) { };


DrawParticles::~DrawParticles() noexcept { }


DrawParticles& DrawParticles::operator=(DrawParticles&& pf_prediction) noexcept
{
    state_model_ = std::move(pf_prediction.state_model_);

    return *this;
}


void DrawParticles::predictStep(const Ref<const MatrixXf>& prev_states, const Ref<const VectorXf>& prev_weights,
                                Ref<MatrixXf> pred_states, Ref<VectorXf> pred_weights)
{
    state_model_->motion(prev_states, pred_states);
    
    pred_weights = prev_weights;
}
