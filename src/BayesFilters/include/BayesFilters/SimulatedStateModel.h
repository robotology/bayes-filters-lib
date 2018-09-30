#ifndef SIMULATEDPROCESS_H
#define SIMULATEDPROCESS_H

#include <BayesFilters/Logger.h>
#include <BayesFilters/Process.h>
#include <BayesFilters/StateModel.h>

#include <memory>
#include <fstream>

namespace bfl {
    class SimulatedStateModel;
}


class bfl::SimulatedStateModel : public Process, public Logger
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

private:
    unsigned int simulation_time_;

    Eigen::MatrixXf target_;

protected:
    std::string process_name_ = "SimulatedProcess";

    std::unique_ptr<StateModel> state_model_;

    Data data_simulated_state_model_;

    unsigned int current_simulation_time_ = 0;

    bool log_enabled_ = false;

    std::string prefix_name_;

    mutable std::ofstream log_file_state_;

    std::vector<std::string> log_filenames(const std::string& prefix_path, const std::string& prefix_name) override
    {
        return {prefix_path + "/" + prefix_name + "_target"};
    }
};

#endif /* SIMULATEDPROCESS_H */
