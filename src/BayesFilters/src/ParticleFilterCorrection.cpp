#include <cmath>
#include <utility>

#include "BayesFilters/ParticleFilterCorrection.h"

using namespace bfl;
using namespace Eigen;


ParticleFilterCorrection::ParticleFilterCorrection(std::unique_ptr<ObservationModel> measurement_model) noexcept :
    Correction(std::move(measurement_model)) { }


ParticleFilterCorrection::~ParticleFilterCorrection() noexcept { }


ParticleFilterCorrection::ParticleFilterCorrection(ParticleFilterCorrection&& pf_correction) noexcept :
    Correction::Correction(std::move(pf_correction)) { };


ParticleFilterCorrection& ParticleFilterCorrection::operator=(ParticleFilterCorrection&& pf_correction) noexcept
{
    Correction::operator=(std::move(pf_correction));

    return *this;
}


void ParticleFilterCorrection::correct(Eigen::Ref<Eigen::VectorXf> states, Eigen::Ref<Eigen::VectorXf> weights, const Eigen::Ref<const Eigen::MatrixXf>& measurements)
{
    Vector2f innovate;
    innovation(states, measurements, innovate);
    likelihood(innovate, weights);
}


void ParticleFilterCorrection::innovation(const Eigen::Ref<const Eigen::VectorXf>& states, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::MatrixXf> innovation)
{
    Vector2f virtual_measurements;

    observe(states, measurements);

    innovation = measurements - measurements;
}


void ParticleFilterCorrection::likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations, Eigen::Ref<Eigen::VectorXf> weights)
{
    weights = (- 0.5 * static_cast<float>(innovations.rows()) * log(2.0*M_PI) - 0.5 * log(obs_model_->noiseCovariance().determinant()) - 0.5 * (innovations.transpose() * obs_model_->noiseCovariance().inverse() * innovations).array()).exp();
}


bool ParticleFilterCorrection::setObservationModelProperty(const std::string& property)
{
    return obs_model_->setProperty(property);
}
