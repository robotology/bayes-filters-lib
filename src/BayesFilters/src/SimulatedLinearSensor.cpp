#include <BayesFilters/SimulatedLinearSensor.h>

using namespace bfl;
using namespace Eigen;


SimulatedLinearSensor::SimulatedLinearSensor
(
    std::unique_ptr<SimulatedStateModel> simulated_state_model,
    const double sigma_x,
    const double sigma_y,
    const unsigned int seed
) :
    LinearModel(sigma_x, sigma_y, seed),
    simulated_state_model_(std::move(simulated_state_model))
{
    /* Since a LinearSensor is intended as a sensor that measure
       the state directly (i.e. no linear combination of the states),
       then it is possible to extract the output size from the
       output size of the simulated state model. */
    std::size_t dim_linear_state;
    std::size_t dim_circular_state;
    std::tie(dim_linear_state, dim_circular_state) = simulated_state_model_->getStateModel().getOutputSize();

    dim_linear_ = 0;
    dim_circular_ = 0;

    for (std::size_t i = 0; i < H_.rows(); i++)
    {
        Eigen::MatrixXf::Index state_index;
        H_.row(i).array().abs().maxCoeff(&state_index);
        if (state_index < dim_linear_state)
            dim_linear_++;
        else
            dim_circular_++;
    }
}


SimulatedLinearSensor::SimulatedLinearSensor
(
     std::unique_ptr<SimulatedStateModel> simulated_state_model,
     const double sigma_x,
     const double sigma_y
) :
    SimulatedLinearSensor(std::move(simulated_state_model), sigma_x, sigma_y, 1)
{ }


SimulatedLinearSensor::SimulatedLinearSensor(std::unique_ptr<SimulatedStateModel> simulated_state_model) :
    SimulatedLinearSensor(std::move(simulated_state_model), 10.0, 10.0, 1)
{ }


SimulatedLinearSensor::~SimulatedLinearSensor() noexcept
{ }


bool SimulatedLinearSensor::freezeMeasurements()
{
    if (!simulated_state_model_->bufferData())
        return false;

    measurement_ = H_ * any::any_cast<MatrixXd>(simulated_state_model_->getData());

    MatrixXd noise;
    std::tie(std::ignore, noise) = getNoiseSample(measurement_.cols());

    measurement_ += noise;

    logger(measurement_.transpose());

    return true;
}


std::pair<bool, Data> SimulatedLinearSensor::measure() const
{
    return std::make_pair(true, measurement_);
}


std::pair<std::size_t, std::size_t> SimulatedLinearSensor::getOutputSize() const
{
    return std::make_pair(dim_linear_, dim_circular_);
}
