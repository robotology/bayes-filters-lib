#ifndef SIMULATEDPROCESS_H
#define SIMULATEDPROCESS_H

#include <BayesFilters/Agent.h>
#include <BayesFilters/Logger.h>
#include <BayesFilters/StateModel.h>

#include <memory>
#include <fstream>

namespace bfl {
    class SimulatedStateModel;
}


class bfl::SimulatedStateModel : public Agent, public Logger
{
public:
    SimulatedStateModel(std::unique_ptr<StateModel> state_model,
                        const Eigen::Ref<const Eigen::Vector4f>& initial_state,
                        const unsigned int simulation_time);

    virtual ~SimulatedStateModel() noexcept;

    bool bufferData() override;

    Data getData() const override;

    bool setProperty(const std::string& property) override;

private:
    unsigned int simulation_time_;

    Eigen::MatrixXf target_;

protected:
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
