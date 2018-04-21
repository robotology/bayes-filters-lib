#include <BayesFilters/StateModelTargetMeasurement.h>

using namespace bfl;
using namespace Eigen;


StateModelTargetMeasurement::StateModelTargetMeasurement
(
    std::unique_ptr<MeasurementModel> measurement_model,
    std::unique_ptr<StateModel> state_model,
    const unsigned int simulation_time
) noexcept :
    MeasurementModelDecorator(std::move(measurement_model)),
    simulation_time_(simulation_time),
    state_model_(std::move(state_model))
{ }


std::pair<bool, MatrixXf> StateModelTargetMeasurement::getMeasurements()
{
    /* FIXME
       Multi-sensor data are WIP.
       Need to properly think how to model them. */

    measurement_.resize(2, simulation_time_);
    target_.resize(4, simulation_time_);

    MatrixXf tmp_measure;

    target_.col(0) << 0, 10, 0, 10;
    std::tie(std::ignore, tmp_measure) = measure(target_.col(0));
    measurement_.col(0) = tmp_measure.col(0);
    for (int k = 1; k < simulation_time_; ++k)
    {
        state_model_->motion(target_.col(k-1), target_.col(k));

        std::tie(std::ignore, tmp_measure) = measure(target_.col(k));
        measurement_.col(k) = tmp_measure.col(0);
    }

    return std::make_tuple(true, measurement_);
}
