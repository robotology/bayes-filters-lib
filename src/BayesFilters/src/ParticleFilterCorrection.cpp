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


void ParticleFilterCorrection::correct(Ref<VectorXf> states, Ref<VectorXf> weights, const Ref<const MatrixXf>& measurements)
{
    Vector2f innovate;
    innovation(states, measurements, innovate);
    likelihood(innovate, weights);
}


void ParticleFilterCorrection::innovation(const Ref<const VectorXf>& states, const Ref<const MatrixXf>& measurements, Ref<MatrixXf> innovation)
{
    Vector2f virtual_measurements;

    obs_model_->observe(states, measurements);

    innovation = measurements - measurements;
}


void ParticleFilterCorrection::likelihood(const Ref<const MatrixXf>& innovations, Ref<VectorXf> weights)
{
    weights = (- 0.5 * static_cast<float>(innovations.rows()) * log(2.0*M_PI) - 0.5 * log(obs_model_->noiseCovariance().determinant()) - 0.5 * (innovations.transpose() * obs_model_->noiseCovariance().inverse() * innovations).array()).exp();
}


bool ParticleFilterCorrection::setModelProperty(const std::string& property)
{
    return obs_model_->setProperty(property);
}
