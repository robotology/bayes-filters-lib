#include <cmath>
#include <utility>

#include "BayesFilters/UpdateParticles.h"

using namespace bfl;
using namespace Eigen;


UpdateParticles::UpdateParticles(std::unique_ptr<ObservationModel> obs_model) noexcept :
    obs_model_(std::move(obs_model)) { }


UpdateParticles::~UpdateParticles() noexcept { }


UpdateParticles::UpdateParticles(UpdateParticles&& pf_correction) noexcept :
    obs_model_(std::move(pf_correction.obs_model_)) { };


UpdateParticles& UpdateParticles::operator=(UpdateParticles&& pf_correction) noexcept
{
    obs_model_ = std::move(pf_correction.obs_model_);

    return *this;
}


void UpdateParticles::correct(Ref<VectorXf> states, Ref<VectorXf> weights, const Ref<const MatrixXf>& measurements)
{
    Vector2f innovate;
    innovation(states, measurements, innovate);
    likelihood(innovate, weights);
}


void UpdateParticles::innovation(const Ref<const VectorXf>& states, const Ref<const MatrixXf>& measurements, Ref<MatrixXf> innovation)
{
    Vector2f virtual_measurements;

    obs_model_->observe(states, virtual_measurements);

    innovation = measurements - virtual_measurements;
}


void UpdateParticles::likelihood(const Ref<const MatrixXf>& innovations, Ref<VectorXf> weights)
{
    weights = (- 0.5 * static_cast<float>(innovations.rows()) * log(2.0*M_PI) - 0.5 * log(obs_model_->getNoiseCovariance().determinant()) - 0.5 * (innovations.transpose() * obs_model_->getNoiseCovariance().inverse() * innovations).array()).exp();
}

ObservationModel UpdateParticles::getObservationModel()
{
    return *obs_model_;
}
