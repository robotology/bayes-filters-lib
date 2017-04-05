#include <cmath>
#include <utility>

#include "BayesFiltersLib/ParticleFilterCorrection.h"

using namespace bfl;
using namespace Eigen;


ParticleFilterCorrection::ParticleFilterCorrection(std::unique_ptr<ObservationModel> measurement_model) noexcept :
    measurement_model_(std::move(measurement_model)) { }


ParticleFilterCorrection::~ParticleFilterCorrection() noexcept { }


ParticleFilterCorrection::ParticleFilterCorrection(ParticleFilterCorrection&& pf_correction) noexcept :
    measurement_model_(std::move(pf_correction.measurement_model_)) { };


ParticleFilterCorrection& ParticleFilterCorrection::operator=(ParticleFilterCorrection&& pf_correction) noexcept
{
    measurement_model_ = std::move(pf_correction.measurement_model_);

    return *this;
}


void ParticleFilterCorrection::correct(const Ref<const VectorXf>& pred_state, const Ref<const MatrixXf>& measurements, Ref<VectorXf> cor_state)
{
    Vector2f innovate;
    innovation(pred_state, measurements, innovate);
    likelihood(innovate, cor_state);
}


void ParticleFilterCorrection::virtual_observation(const Ref<const VectorXf>& state, Ref<MatrixXf> virtual_measurements)
{
    measurement_model_->observe(state, virtual_measurements);
}


void ParticleFilterCorrection::innovation(const Ref<const VectorXf>& pred_state, const Ref<const MatrixXf>& measurements, Ref<MatrixXf> innovation)
{
    Vector2f virtual_measurements;

    virtual_observation(pred_state, virtual_measurements);

    innovation = measurements - virtual_measurements;
}


void ParticleFilterCorrection::likelihood(const Ref<const MatrixXf>& innovation, Ref<VectorXf> cor_state)
{
    cor_state = (- 0.5 * static_cast<float>(innovation.rows()) * log(2.0*M_PI) - 0.5 * log(measurement_model_->noiseCovariance().determinant()) - 0.5 * (innovation.transpose() * measurement_model_->noiseCovariance().inverse() * innovation).array()).exp();
}


/* TEMPORARY - WILL BE REMOVED AFTER IMPLEMENTING Initialization CLASS */
void ParticleFilterCorrection::observation(const Eigen::Ref<const Eigen::VectorXf>& state, Eigen::Ref<Eigen::MatrixXf> measurements)
{
    measurement_model_->measure(state, measurements);
}
