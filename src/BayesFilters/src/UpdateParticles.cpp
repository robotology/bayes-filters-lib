#include "BayesFilters/UpdateParticles.h"

#include <cmath>
#include <utility>

using namespace bfl;
using namespace Eigen;


UpdateParticles::UpdateParticles() noexcept { }


UpdateParticles::UpdateParticles(UpdateParticles&& pf_correction) noexcept :
    PFCorrection(std::move(pf_correction)) { };


UpdateParticles::~UpdateParticles() noexcept { }


void UpdateParticles::correctStep(const Ref<const MatrixXf>& pred_states, const Ref<const VectorXf>& pred_weights, const Ref<const MatrixXf>& measurements,
                                  Ref<MatrixXf> cor_states, Ref<VectorXf> cor_weights)
{
    MatrixXf innovations(measurements.rows(), pred_states.cols());
    innovation(pred_states, measurements, innovations);

    for (unsigned int i = 0; i < innovations.cols(); ++i)
        cor_weights(i) = likelihood(innovations.col(i));

    cor_states = pred_states;
}


void UpdateParticles::innovation(const Ref<const MatrixXf>& pred_states, const Ref<const MatrixXf>& measurements, Ref<MatrixXf> innovations)
{
    MatrixXf virtual_measurements(measurements.rows(), pred_states.cols());
    observation_model_->observe(pred_states, virtual_measurements);

    innovations = virtual_measurements.colwise() - measurements.col(0);
}


double UpdateParticles::likelihood(const Ref<const VectorXf>& innovation)
{
    return (- 0.5 * static_cast<float>(innovation.rows()) * log(2.0*M_PI) - 0.5 * log(observation_model_->getNoiseCovarianceMatrix().determinant()) - 0.5 * (innovation.transpose() * observation_model_->getNoiseCovarianceMatrix().inverse() * innovation).array()).exp().cast<double>().coeff(0);
}


ObservationModel& UpdateParticles::getObservationModel()
{
    return *observation_model_;
}


void UpdateParticles::setObservationModel(std::unique_ptr<ObservationModel> observation_model)
{
    observation_model_ = std::move(observation_model);
}
