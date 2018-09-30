#ifndef SIMULATEDPROCESS_H
#define SIMULATEDPROCESS_H

#include <BayesFilters/Process.h>
#include <BayesFilters/StateModel.h>

#include <memory>
#include <fstream>

namespace bfl {
    class SimulatedStateModel;
}


class bfl::SimulatedStateModel : public Process
{
public:
    SimulatedStateModel(std::unique_ptr<StateModel> state_model,
                        const Eigen::Ref<const Eigen::Vector4f>& initial_state,
                        const unsigned int simulation_time);

    SimulatedStateModel(std::unique_ptr<StateModel> state_model,
                        const Eigen::Ref<const Eigen::Vector4f>& initial_state,
                        const unsigned int simulation_time,
                        std::string process_name);

    virtual ~SimulatedStateModel() noexcept;

    bool bufferProcessData() override;

    Data getProcessData() const override;

    bool setProperty(const std::string& property) override;

    void enableLog(const std::string& prefix_name) override;
private:
    unsigned int simulation_time_;

    void disableLog() override;
    Eigen::MatrixXf target_;

protected:
    std::string process_name_ = "SimulatedProcess";

    std::unique_ptr<StateModel> state_model_;

    Data data_simulated_state_model_;

    unsigned int current_simulation_time_ = 0;

    bool log_enabled_ = false;

    std::string prefix_name_;

    mutable std::ofstream log_file_state_;

    void logger(const Eigen::Ref<const Eigen::MatrixXf>& data) const;
};

#endif /* SIMULATEDPROCESS_H */
