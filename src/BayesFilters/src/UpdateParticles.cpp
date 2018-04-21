#include "BayesFilters/UpdateParticles.h"

#include <cmath>
#include <utility>

using namespace bfl;
using namespace Eigen;


UpdateParticles::UpdateParticles() noexcept { }


UpdateParticles::UpdateParticles(UpdateParticles&& pf_correction) noexcept :
    PFCorrection(std::move(pf_correction)) { };


UpdateParticles::~UpdateParticles() noexcept { }


std::pair<bool, VectorXf> UpdateParticles::getLikelihood()
{
    return std::make_pair(valid_likelihood_, likelihood_);
}


void UpdateParticles::correctStep(const Ref<const MatrixXf>& pred_states, const Ref<const VectorXf>& pred_weights,
                                  Ref<MatrixXf> cor_states, Ref<VectorXf> cor_weights)
{
    bool valid_measurements;
    MatrixXf measurements;
    std::tie(valid_measurements, measurements) = measurement_model_->getMeasurements();

    bool valid_predicted_measurements;
    MatrixXf predicted_measurements;
    std::tie(valid_predicted_measurements, predicted_measurements) = measurement_model_->predictedMeasure(pred_states);

    bool valid_innovation;
    MatrixXf innovations;
    std::tie(valid_innovation, innovations) = measurement_model_->innovation(predicted_measurements, measurements);

    std::tie(valid_likelihood_, likelihood_) = likelihood(innovations);


    cor_states = pred_states;
    (valid_measurements && valid_predicted_measurements && valid_innovation && valid_likelihood_) ?
        cor_weights = likelihood_ : cor_weights = pred_weights;
}


std::pair<bool, Eigen::VectorXf> UpdateParticles::likelihood(const Ref<const MatrixXf>& innovations)
{
    return std::make_pair(false, VectorXf::Zero(1));
}
