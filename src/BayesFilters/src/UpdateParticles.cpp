#include <BayesFilters/UpdateParticles.h>

#include <cmath>
#include <utility>

using namespace bfl;
using namespace Eigen;


UpdateParticles::UpdateParticles() noexcept { }


UpdateParticles::~UpdateParticles() noexcept { }


void UpdateParticles::correctStep(const ParticleSet& pred_particles, ParticleSet& cor_particles)
{
    bool valid_freeze = measurement_model_->freezeMeasurements();

    if (valid_freeze)
        std::tie(valid_likelihood_, likelihood_) = likelihood_model_->likelihood(*measurement_model_,
                                                                                 pred_particles.state().cast<float>());
    cor_particles = pred_particles;

    if (valid_likelihood_)
        cor_particles.weight() = cor_particles.weight().cwiseProduct(likelihood_.cast<double>());
}


std::pair<bool, VectorXf> UpdateParticles::getLikelihood()
{
    return std::make_pair(valid_likelihood_, likelihood_);
}


void UpdateParticles::setLikelihoodModel(std::unique_ptr<LikelihoodModel> likelihood_model)
{
    likelihood_model_ = std::move(likelihood_model);
}


void UpdateParticles::setMeasurementModel(std::unique_ptr<MeasurementModel> measurement_model)
{
    measurement_model_ = std::move(measurement_model);
}


LikelihoodModel& UpdateParticles::getLikelihoodModel()
{
    return *likelihood_model_;
}


MeasurementModel& UpdateParticles::getMeasurementModel()
{
    return *measurement_model_;
}
