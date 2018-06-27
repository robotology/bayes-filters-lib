#include <BayesFilters/SimulatedProcess.h>

#include <iostream>

using namespace bfl;
using namespace Eigen;


SimulatedProcess::SimulatedProcess
(
    std::unique_ptr<StateModel> state_model,
    const Ref<const Vector4f>& initial_state,
    const unsigned int simulation_time
) :
    simulation_time_(simulation_time),
    state_model_(std::move(state_model))
{
    target_ = MatrixXf(4, simulation_time_);
    target_.col(0) = initial_state;

    for (int k = 1; k < simulation_time_; ++k)
        state_model_->motion(target_.col(k - 1), target_.col(k));
}


SimulatedProcess::SimulatedProcess
(
    std::unique_ptr<StateModel> state_model,
    const Ref<const Vector4f>& initial_state,
    const unsigned int simulation_time,
    std::string process_name
) :
    SimulatedProcess(std::move(state_model), initial_state, simulation_time)
{
    process_name_ = process_name_;
}


SimulatedProcess::~SimulatedProcess()
{
    if (log_enabled_)
        disableLog();
}


bool SimulatedProcess::bufferProcessState()
{
    ++current_simulation_time_;

    return true;
}


std::pair<bool, MatrixXf> SimulatedProcess::getProcessState() const
{
    if (log_enabled_)
        logger(target_.col(current_simulation_time_ - 1));

    return std::make_pair(true, target_.col(current_simulation_time_ - 1));
}


bool SimulatedProcess::setProperty(const std::string property)
{
    if (property == "reset")
    {
        current_simulation_time_ = 0;
        std::cout << "Successfully reset " + process_name_ + "." << std::endl;

        return true;
    }

    return false;
}


void SimulatedProcess::enableLog(const std::string& prefix_name)
{
    prefix_name_ = prefix_name;

    log_file_state_.open("./" + prefix_name_ + "_process_state.txt", std::ofstream::out | std::ofstream::app);

    log_enabled_ = true;
}


void SimulatedProcess::disableLog()
{
    log_enabled_ = false;

    log_file_state_.close();
}


void SimulatedProcess::logger(const Ref<const MatrixXf>& data) const
{
    log_file_state_ << data.transpose() << std::endl;
}