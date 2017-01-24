#include <cmath>
#include <utility>

#include "BayesFiltersLib/ParticleFilterCorrection.h"

using namespace Eigen;


namespace bfl
{

ParticleFilterCorrection::ParticleFilterCorrection(std::shared_ptr<ObservationModel> measurement_model) noexcept :
    measurement_model_(measurement_model) { }


ParticleFilterCorrection::~ParticleFilterCorrection() noexcept { }


ParticleFilterCorrection::ParticleFilterCorrection(const ParticleFilterCorrection& pf_correction)
{
    measurement_model_ = pf_correction.measurement_model_;
}


ParticleFilterCorrection::ParticleFilterCorrection(ParticleFilterCorrection&& pf_correction) noexcept :
    measurement_model_(std::move(pf_correction.measurement_model_)) { };


ParticleFilterCorrection& ParticleFilterCorrection::operator=(const ParticleFilterCorrection& pf_correction)
{
    ParticleFilterCorrection tmp(pf_correction);
    *this = std::move(tmp);

    return *this;
}


ParticleFilterCorrection& ParticleFilterCorrection::operator=(ParticleFilterCorrection&& pf_correction) noexcept
{
    measurement_model_ = std::move(pf_correction.measurement_model_);

    return *this;
}


void ParticleFilterCorrection::correct(const Eigen::Ref<const Eigen::VectorXf>& pred_state, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::VectorXf> cor_state)
{
    Vector2f innovate;
    innovation(pred_state, measurements, innovate);
    likelihood(innovate, cor_state);
}

void ParticleFilterCorrection::innovation(const Eigen::Ref<const Eigen::VectorXf>& pred_state, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::MatrixXf> innovation)
{
    Vector2f virtual_measurement;
    measurement_model_->observe(pred_state, virtual_measurement);

    innovation = measurements - virtual_measurement;
}

void ParticleFilterCorrection::likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovation, Eigen::Ref<Eigen::VectorXf> cor_state)
{
    cor_state = (- 0.5 * static_cast<float>(innovation.rows()) * log(2.0*M_PI) - 0.5 * log(measurement_model_->noiseCovariance().determinant()) - 0.5 * (innovation.transpose() * measurement_model_->noiseCovariance().inverse() * innovation).array()).exp();
}

} // namespace bfl
