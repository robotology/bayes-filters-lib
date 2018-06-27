#ifndef SIMULATEDPROCESS_H
#define SIMULATEDPROCESS_H

#include <BayesFilters/Process.h>
#include <BayesFilters/StateModel.h>

#include <memory>
#include <fstream>

namespace bfl {
    class SimulatedProcess;
}


class bfl::SimulatedProcess : public Process
{
public:
    SimulatedProcess(std::unique_ptr<StateModel> state_model,
                     const Eigen::Ref<const Eigen::Vector4f>& initial_state,
                     const unsigned int simulation_time);

    SimulatedProcess(std::unique_ptr<StateModel> state_model,
                     const Eigen::Ref<const Eigen::Vector4f>& initial_state,
                     const unsigned int simulation_time,
                     std::string process_name);

    virtual ~SimulatedProcess() noexcept;

    bool bufferProcessState() override;

    std::pair<bool, Eigen::MatrixXf> getProcessState() const override;

    bool setProperty(const std::string property) override;


    void enableLog(const std::string& prefix_name) override;

    void disableLog() override;

public:
    unsigned int simulation_time_;

    std::string process_name_ = "SimulatedProcess";

    Eigen::MatrixXf target_;

    Eigen::MatrixXf measurement_;

private:
    std::unique_ptr<StateModel> state_model_;

    unsigned int current_simulation_time_ = 0;

    bool log_enabled_ = false;

    std::string prefix_name_;

    mutable std::ofstream log_file_state_;

    void log(const Eigen::Ref<const Eigen::MatrixXf>& data) const;
};

#endif /* SIMULATEDPROCESS_H */
