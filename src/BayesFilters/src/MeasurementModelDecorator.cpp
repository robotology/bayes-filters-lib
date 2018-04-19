#include <BayesFilters/MeasurementModelDecorator.h>

using namespace bfl;
using namespace Eigen;


MeasurementModelDecorator::MeasurementModelDecorator(std::unique_ptr<MeasurementModel> observation_model) noexcept :
    observation_model_(std::move(observation_model)) { }


MeasurementModelDecorator::MeasurementModelDecorator(MeasurementModelDecorator&& observation_model) noexcept :
    observation_model_(std::move(observation_model.observation_model_)) { }


MeasurementModelDecorator::~MeasurementModelDecorator() noexcept { }


MeasurementModelDecorator& MeasurementModelDecorator::operator=(MeasurementModelDecorator&& observation_model) noexcept
{
    observation_model_ = std::move(observation_model.observation_model_);

    return *this;
}


std::pair<bool, MatrixXf> MeasurementModelDecorator::measure(const Ref<const MatrixXf>& cur_states)
{
    return observation_model_->measure(cur_states);
}


std::pair<bool, MatrixXf> MeasurementModelDecorator::getMeasurements()
{
    return observation_model_->getMeasurements();
}


std::pair<bool, MatrixXf> MeasurementModelDecorator::innovation(const Ref<const MatrixXf>& predicted_measurements, const Ref<const MatrixXf>& measurements)
{
    return observation_model_->innovation(predicted_measurements, measurements);
}


std::pair<bool, MatrixXf> MeasurementModelDecorator::predictedMeasure(const Ref<const MatrixXf>& cur_states)
{
    return observation_model_->predictedMeasure(cur_states);
}


std::pair<bool, MatrixXf> MeasurementModelDecorator::getNoiseSample(const int num)
{
    return observation_model_->getNoiseSample(num);
}


std::pair<bool, MatrixXf> MeasurementModelDecorator::getNoiseCovarianceMatrix()
{
    return observation_model_->getNoiseCovarianceMatrix();
}


bool MeasurementModelDecorator::setProperty(const std::string property)
{
    return observation_model_->setProperty(property);
}
