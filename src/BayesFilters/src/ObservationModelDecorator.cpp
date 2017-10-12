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


void ObservationModelDecorator::observe(const Ref<const MatrixXf>& cur_state, Ref<MatrixXf> observation)
{
    observation_model_->observe(cur_state, observation);
}


void ObservationModelDecorator::measure(const Ref<const MatrixXf>& cur_state, Ref<MatrixXf> measurement)
{
    observation_model_->measure(cur_state, measurement);
}


MatrixXf ObservationModelDecorator::getNoiseSample(const int num)
{
    return observation_model_->getNoiseSample(num);
}


MatrixXf ObservationModelDecorator::getNoiseCovariance()
{
    return observation_model_->getNoiseCovariance();
}


bool ObservationModelDecorator::setProperty(const std::string property)
{
    return observation_model_->setProperty(property);
}
