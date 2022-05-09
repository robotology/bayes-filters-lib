/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/SimulatedLinearSensor.h>
#include <BayesFilters/VectorDescription.h>

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
     * then it is possible to extract input and measurement descriptions
     * from the state description of the simulated state model.
     */

    /* Input description. */
    input_description_ = simulated_state_model_->getStateModel().getStateDescription();

    /* The number of noise components depend on the size of the noise covariance matrix. */
    MatrixXd noise_covariance;
    bool valid_noise_covariance;
    std::tie(valid_noise_covariance, noise_covariance) = getNoiseCovarianceMatrix();
    if (!valid_noise_covariance)
        throw(std::runtime_error("ERROR:SIMULATEDLINEARSENSOR::CTOR. A valid noise covariance matrix from getNoiseCovarianceMatrix() is required."));
    input_description_.add_noise_components(noise_covariance.rows());

    /* Measurement description.
     *
     * FIXME:
     * Check if this works in presence of quaternions too.
     */
    std::size_t meas_linear_size = 0;
    std::size_t meas_circular_size = 0;

    for (std::size_t i = 0; i < H_.rows(); ++i)
    {
        Eigen::MatrixXf::Index state_index;
        H_.row(i).array().abs().maxCoeff(&state_index);

        if (state_index < input_description_.linear_size())
            meas_linear_size++;
        else
            meas_circular_size++;
    }
    measurement_description_ = VectorDescription(meas_linear_size, meas_circular_size);
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


VectorDescription SimulatedLinearSensor::getInputDescription() const
{
    return input_description_;
}


VectorDescription SimulatedLinearSensor::getMeasurementDescription() const
{
    return measurement_description_;
}


void SimulatedLinearSensor::log()
{
    logger(measurement_.transpose());
}
