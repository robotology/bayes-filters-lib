/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/SimulatedLinearSensor.h>

using namespace bfl;
using namespace Eigen;


SimulatedLinearSensor::~SimulatedLinearSensor() noexcept = default;


SimulatedLinearSensor::SimulatedLinearSensor
(
    std::unique_ptr<SimulatedStateModel> simulated_state_model,
    const LinearMatrixComponent& linear_matrix_component,
    const Eigen::Ref<const Eigen::MatrixXd>& noise_covariance_matrix,
    const unsigned int seed
) :
    LinearModel(linear_matrix_component, noise_covariance_matrix, seed),
    simulated_state_model_(std::move(simulated_state_model))
{
    /* Since a LinearSensor is intended as a sensor that measure
     * the state directly (i.e. no linear combination of the states),
     * then it is possible to extract the output size from the
     * output size of the simulated state model.
     */
    std::size_t dim_linear_state;
    std::tie(dim_linear_state, std::ignore) = simulated_state_model_->getStateModel().getOutputSize();

    dim_linear_ = 0;
    dim_circular_ = 0;

    for (std::size_t i = 0; i < H_.rows(); ++i)
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
    const LinearMatrixComponent& linear_matrix_component,
    const Eigen::Ref<const Eigen::MatrixXd>& noise_covariance_matrix
) :
    SimulatedLinearSensor(std::move(simulated_state_model), linear_matrix_component, noise_covariance_matrix, 1)
{ }


bool SimulatedLinearSensor::freeze(const Data& data)
{
    if (!simulated_state_model_->bufferData())
        return false;

    measurement_ = H_ * any::any_cast<MatrixXd>(simulated_state_model_->getData());

    MatrixXd noise;
    std::tie(std::ignore, noise) = getNoiseSample(measurement_.cols());

    measurement_ += noise;

    log();

    return true;
}


std::pair<bool, Data> SimulatedLinearSensor::measure(const Data& data) const
{
    return std::make_pair(true, measurement_);
}


std::pair<std::size_t, std::size_t> SimulatedLinearSensor::getOutputSize() const
{
    return std::make_pair(dim_linear_, dim_circular_);
}


void SimulatedLinearSensor::log()
{
    logger(measurement_.transpose());
}
