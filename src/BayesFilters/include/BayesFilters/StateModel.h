/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef STATEMODEL_H
#define STATEMODEL_H

#include <BayesFilters/ExogenousModel.h>
#include <BayesFilters/Skippable.h>
#include <BayesFilters/StateProcess.h>

#include <memory>

namespace bfl {
    class StateModel;
}


class bfl::StateModel : public StateProcess, public Skippable
{
public:
    virtual ~StateModel() noexcept = default;

    bool skip(const std::string& what_step, const bool status) override;

    bool getSkipState() override;

    bool add_exogenous_model(std::unique_ptr<ExogenousModel> exogenous_model);

    bool have_exogenous_model() noexcept;

    ExogenousModel& exogenous_model();

    virtual Eigen::MatrixXd getJacobian();

    virtual Eigen::VectorXd getTransitionProbability(const Eigen::Ref<const Eigen::MatrixXd>& prev_states, const Eigen::Ref<const Eigen::MatrixXd>& cur_states);

    virtual Eigen::MatrixXd getNoiseCovarianceMatrix();

    virtual Eigen::MatrixXd getNoiseSample(const std::size_t num);


protected:
    StateModel() noexcept = default;

    StateModel(const StateModel& state_model) noexcept = delete;

    StateModel& operator=(const StateModel& state_model) noexcept = delete;

    StateModel(StateModel&& state_model) noexcept = default;

    StateModel& operator=(StateModel&& state_model) noexcept = default;


private:
    /**
     * Skip status.
     */
    bool skip_ = false;

    /**
     * Exogenous model.
     */
    std::unique_ptr<ExogenousModel> exogenous_model_;
};

#endif /* STATEMODEL_H */
