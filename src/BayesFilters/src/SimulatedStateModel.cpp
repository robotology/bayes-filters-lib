#include <BayesFilters/SimulatedStateModel.h>

#include <iostream>

using namespace bfl;
using namespace Eigen;


SimulatedStateModel::SimulatedStateModel
(
    std::unique_ptr<StateModel> state_model,
    const Ref<const Vector4f>& initial_state,
    const unsigned int simulation_time
) :
    simulation_time_(simulation_time),
    state_model_(std::move(state_model)),
    simulated_state_model_data_(std::make_shared<EigenVectorXfData>(VectorXf(4)))
{
    target_ = MatrixXf(4, simulation_time_);
    target_.col(0) = initial_state;

    for (int k = 1; k < simulation_time_; ++k)
        state_model_->motion(target_.col(k - 1), target_.col(k));
}


SimulatedStateModel::SimulatedStateModel
(
    std::unique_ptr<StateModel> state_model,
    const Ref<const Vector4f>& initial_state,
    const unsigned int simulation_time,
    std::string process_name
) :
    SimulatedStateModel(std::move(state_model), initial_state, simulation_time)
{
    process_name_ = process_name_;
}


SimulatedStateModel::~SimulatedStateModel() noexcept
{
    if (log_enabled_)
        disableLog();
}


bool SimulatedStateModel::bufferProcessData()
{
    ++current_simulation_time_;

    if (log_enabled_)
        logger(target_.col(current_simulation_time_ - 1));

    (*simulated_state_model_data_) = target_.col(current_simulation_time_ - 1);

    return true;
}


std::shared_ptr<GenericData> SimulatedStateModel::getProcessData()
{
    return simulated_state_model_data_;
}


bool SimulatedStateModel::setProperty(const std::string& property)
{
    if (property == "reset")
    {
        current_simulation_time_ = 0;
        std::cout << "Successfully reset " + process_name_ + "." << std::endl;

        return true;
    }

    return false;
}


void SimulatedStateModel::enableLog(const std::string& prefix_name)
{
    prefix_name_ = prefix_name;

    log_file_state_.open("./" + prefix_name_ + "_process_state.txt", std::ofstream::out | std::ofstream::app);

    log_enabled_ = true;
}


void SimulatedStateModel::disableLog()
{
    log_enabled_ = false;

    log_file_state_.close();
}


void SimulatedStateModel::logger(const Ref<const MatrixXf>& data) const
{
    log_file_state_ << data.transpose() << std::endl;
}
