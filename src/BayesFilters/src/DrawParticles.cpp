#include <BayesFilters/DrawParticles.h>

#include <utility>

using namespace bfl;
using namespace Eigen;


DrawParticles::DrawParticles() noexcept { }


DrawParticles::DrawParticles(DrawParticles&& draw_particles) noexcept :
    PFPrediction(std::move(draw_particles)) { };


DrawParticles::~DrawParticles() noexcept { }


void DrawParticles::predictStep(const ParticleSet& prev_particles, ParticleSet& pred_particles)
{
    /* Temporary variable required until discrepancy between
       MatrixXf and MatrixXd is solved. */
    MatrixXf tmp(pred_particles.dim, pred_particles.components);
    state_model_->motion(prev_particles.state().cast<float>(), tmp);
    pred_particles.state() = std::move(tmp.cast<double>());

    pred_particles.weight() = prev_particles.weight();
}

StateModel& DrawParticles::getStateModel()
{
    return *state_model_;
}


void DrawParticles::setStateModel(std::unique_ptr<StateModel> state_model)
{
    state_model_ = std::move(state_model);
}
