#include <BayesFilters/SimulatedLinearSensor.h>

using namespace bfl;
using namespace Eigen;


SimulatedLinearSensor::SimulatedLinearSensor
(
    std::unique_ptr<SimulatedStateModel> simulated_state_model,
    const float sigma_x,
    const float sigma_y,
    const unsigned int seed
) :
    LinearModel(sigma_x, sigma_y, seed),
    simulated_state_model_(std::move(simulated_state_model))
{ }


SimulatedLinearSensor::SimulatedLinearSensor
(
     std::unique_ptr<SimulatedStateModel> simulated_state_model,
     const float sigma_x,
     const float sigma_y
) :
    LinearModel(sigma_x, sigma_y),
    simulated_state_model_(std::move(simulated_state_model))
{ }


SimulatedLinearSensor::SimulatedLinearSensor(std::unique_ptr<SimulatedStateModel> simulated_state_model) :
    LinearModel(),
    simulated_state_model_(std::move(simulated_state_model))
{ }


SimulatedLinearSensor::~SimulatedLinearSensor() noexcept
{ }


bool SimulatedLinearSensor::bufferAgentData() const
{
    return simulated_state_model_->bufferData();
}


std::pair<bool, Data> SimulatedLinearSensor::getAgentMeasurements() const
{
    MatrixXf simulated_state = any::any_cast<MatrixXf>(simulated_state_model_->getData());

    return measure(simulated_state);
}
