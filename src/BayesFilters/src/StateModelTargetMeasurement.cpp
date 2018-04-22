#include <BayesFilters/StateModelTargetMeasurement.h>

#include <iostream>
#include <typeinfo>

using namespace bfl;
using namespace Eigen;


StateModelTargetMeasurement::StateModelTargetMeasurement
(
    std::unique_ptr<MeasurementModel> measurement_model,
    std::unique_ptr<StateModel> state_model,
    const Ref<const Vector4f>& initial_state,
    const unsigned int simulation_time
) noexcept :
    MeasurementModelDecorator(std::move(measurement_model)),
    simulation_time_(simulation_time),
    state_model_(std::move(state_model))
{
    target_ = MatrixXf(4, simulation_time_);
    target_.col(0) = initial_state;

    for (int k = 1; k < simulation_time_; ++k)
        state_model_->motion(target_.col(k-1), target_.col(k));


    MatrixXf tmp_measure;
    std::tie(std::ignore, tmp_measure) = measure(target_.col(0));

    measurement_ = MatrixXf(tmp_measure.rows(), simulation_time_);
    measurement_.col(0) = tmp_measure.col(0);

    for (int k = 1; k < simulation_time_; ++k)
    {
        std::tie(std::ignore, tmp_measure) = measure(target_.col(k));
        measurement_.col(k) = tmp_measure.col(0);
    }
}


std::pair<bool, MatrixXf> StateModelTargetMeasurement::getMeasurements()
{
    /* FIXME
       Multi-sensor data are WIP.
       Need to properly think how to model them. */

    return std::make_tuple(true, measurement_.col(measurement_time_++));
}


bool StateModelTargetMeasurement::setProperty(const std::string property)
{
    bool successful_previous_property = MeasurementModelDecorator::setProperty(property);

    if (property == "reset")
    {
        measurement_time_ = 0;
        std::cout << "Successfully reset StateModelTargetMeasurement." << std::endl;
    }

    return successful_previous_property;
}
