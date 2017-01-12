#include <cmath>

#include <FilteringFunction/ParticleFilterCorrection.h>

using namespace Eigen;


ParticleFilterCorrection::ParticleFilterCorrection(std::shared_ptr<ObservationModel> measurement_model) noexcept :
    measurement_model_(measurement_model) { };


ParticleFilterCorrection::~ParticleFilterCorrection() noexcept { };


ParticleFilterCorrection::ParticleFilterCorrection(const ParticleFilterCorrection& pf_correction)
{
    measurement_model_ = pf_correction.measurement_model_;
};


ParticleFilterCorrection::ParticleFilterCorrection(ParticleFilterCorrection&& pf_correction) noexcept :
    measurement_model_(std::move(pf_correction.measurement_model_)) { };


ParticleFilterCorrection& ParticleFilterCorrection::operator=(const ParticleFilterCorrection& pf_correction)
{
    ParticleFilterCorrection tmp(pf_correction);
    *this = std::move(tmp);

    return *this;
};


ParticleFilterCorrection& ParticleFilterCorrection::operator=(ParticleFilterCorrection&& pf_correction) noexcept
{
    measurement_model_ = std::move(pf_correction.measurement_model_);

    return *this;
};


void ParticleFilterCorrection::correct(const Eigen::Ref<const Eigen::VectorXf>& pred_state, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::VectorXf> cor_state)
{
    Vector2f virtual_measurement;
    measurement_model_->observe(pred_state, virtual_measurement);

    /* Simple Linear sensor for position observations */
    Vector2f innovation;
    innovation = measurements - virtual_measurement;

    cor_state = (- 0.5 * static_cast<float>(measurements.rows()) * log(2.0*M_PI) - 0.5 * log(measurement_model_->noiseCovariance().determinant()) - 0.5 * (innovation.transpose() * measurement_model_->noiseCovariance().inverse() * innovation).array()).exp();
}
