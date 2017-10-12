#include "BayesFilters/UpdateParticles.h"

#include <cmath>
#include <utility>

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


void UpdateParticles::correct(const Ref<const MatrixXf>& pred_states, const Ref<const VectorXf>& pred_weights, const Ref<const MatrixXf>& measurements,
                              Ref<MatrixXf> cor_states, Ref<VectorXf> cor_weights)
{
    cor_states = pred_states;

    MatrixXf innovations(measurements.rows(), pred_states.cols());
    innovation(pred_states, measurements, innovations);

    for (unsigned int i = 0; i < innovations.cols(); ++i)
        cor_weights(i) = likelihood(innovations.col(i));
}


void UpdateParticles::innovation(const Ref<const MatrixXf>& pred_states, const Ref<const MatrixXf>& measurements, Ref<MatrixXf> innovations)
{
    MatrixXf virtual_measurements(measurements.rows(), pred_states.cols());
    obs_model_->observe(pred_states, virtual_measurements);

    innovations = virtual_measurements.colwise() - measurements.col(0);
}


double UpdateParticles::likelihood(const Ref<const VectorXf>& innovation)
{
    return (- 0.5 * static_cast<float>(innovation.rows()) * log(2.0*M_PI) - 0.5 * log(obs_model_->getNoiseCovariance().determinant()) - 0.5 * (innovation.transpose() * obs_model_->getNoiseCovariance().inverse() * innovation).array()).exp().cast<double>().coeff(0);
}

ObservationModel& UpdateParticles::getObservationModel()
{
    return *obs_model_;
}
