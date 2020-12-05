/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/SimulatedStateModel.h>

#include <iostream>

using namespace bfl;
using namespace Eigen;


SimulatedStateModel::SimulatedStateModel
(
    std::unique_ptr<StateModel> state_model,
    const Ref<const VectorXd>& initial_state,
    const unsigned int simulation_time
) :
    simulation_time_(simulation_time),
    state_model_(std::move(state_model))
{
    target_ = MatrixXd(initial_state.rows(), simulation_time_);
    target_.col(0) = initial_state;

    for (int k = 1; k < simulation_time_; ++k)
        state_model_->motion(target_.col(k - 1), target_.col(k));
}


bool SimulatedStateModel::bufferData()
{
    ++current_simulation_time_;

    log();

    MatrixXd process_information = target_.col(current_simulation_time_ - 1);

    data_simulated_state_model_ = std::move(process_information);

    return true;
}


Data SimulatedStateModel::getData() const
{
    return data_simulated_state_model_;
}


bool SimulatedStateModel::setProperty(const std::string& property)
{
    if (property == "reset")
    {
        current_simulation_time_ = 0;
        std::cout << "Successfully reset state model." << std::endl;

        return true;
    }

    return false;
}


StateModel& SimulatedStateModel::getStateModel()
{
    return *state_model_;
}


void SimulatedStateModel::log()
{
    logger(target_.col(current_simulation_time_ - 1).transpose());
}
