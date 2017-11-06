#include "BayesFilters/ObservationModelDecorator.h"

using namespace bfl;
using namespace Eigen;


ObservationModelDecorator::ObservationModelDecorator(std::unique_ptr<ObservationModel> observation_model) noexcept :
    observation_model_(std::move(observation_model)) { }


ObservationModelDecorator::ObservationModelDecorator(ObservationModelDecorator&& observation_model) noexcept :
    observation_model_(std::move(observation_model.observation_model_)) { }

ObservationModelDecorator::~ObservationModelDecorator() noexcept { }


ObservationModelDecorator& ObservationModelDecorator::operator=(ObservationModelDecorator&& observation_model) noexcept
{
    observation_model_ = std::move(observation_model.observation_model_);

    return *this;
}


void ObservationModelDecorator::observe(const Ref<const MatrixXf>& cur_states, Ref<MatrixXf> observations)
{
    observation_model_->observe(cur_states, observations);
}


void ObservationModelDecorator::measure(const Ref<const MatrixXf>& cur_states, Ref<MatrixXf> measurements)
{
    observation_model_->measure(cur_states, measurements);
}


MatrixXf ObservationModelDecorator::getNoiseSample(const int num)
{
    return observation_model_->getNoiseSample(num);
}


MatrixXf ObservationModelDecorator::getNoiseCovarianceMatrix()
{
    return observation_model_->getNoiseCovarianceMatrix();
}


bool ObservationModelDecorator::setProperty(const std::string property)
{
    return observation_model_->setProperty(property);
}
