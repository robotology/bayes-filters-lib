#include "BayesFilters/DrawParticles.h"

#include <utility>

using namespace bfl;
using namespace Eigen;


DrawParticles::DrawParticles() noexcept { }


DrawParticles::DrawParticles(DrawParticles&& draw_particles) noexcept :
    PFPrediction(std::move(draw_particles)) { };


DrawParticles::~DrawParticles() noexcept { }


void DrawParticles::predictStep(const Ref<const MatrixXf>& prev_states, const Ref<const VectorXf>& prev_weights,
                                Ref<MatrixXf> pred_states, Ref<VectorXf> pred_weights)
{
    state_model_->motion(prev_states, pred_states);
    
    pred_weights = prev_weights;
}
