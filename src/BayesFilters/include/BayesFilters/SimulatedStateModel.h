/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

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
    SimulatedStateModel(std::unique_ptr<StateModel> state_model, const Eigen::Ref<const Eigen::VectorXd>& initial_state, const unsigned int simulation_time);

    virtual ~SimulatedStateModel() noexcept = default;

    bool bufferData() override;

    Data getData() const override;

    bool setProperty(const std::string& property) override;

    StateModel& getStateModel();

private:
    unsigned int simulation_time_;

    Eigen::MatrixXd target_;

protected:
    std::unique_ptr<StateModel> state_model_;

    Data data_simulated_state_model_;

    unsigned int current_simulation_time_ = 0;

    bool log_enabled_ = false;

    mutable std::ofstream log_file_state_;

    std::vector<std::string> log_file_names(const std::string& folder_path, const std::string& file_name_prefix) override
    {
        return {folder_path + "/" + file_name_prefix + "_target"};
    }

    void log() override;
};

#endif /* SIMULATEDPROCESS_H */
