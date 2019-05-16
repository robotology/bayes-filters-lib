/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef LINEARSTATEMODEL_H
#define LINEARSTATEMODEL_H

#include <BayesFilters/AdditiveStateModel.h>

namespace bfl {
    class LinearStateModel;
}


class bfl::LinearStateModel : public bfl::AdditiveStateModel
{
public:
    virtual ~LinearStateModel() noexcept = default;

    virtual void propagate(const Eigen::Ref<const Eigen::MatrixXd>& cur_states, Eigen::Ref<Eigen::MatrixXd> prop_states) override;

    virtual Eigen::MatrixXd getStateTransitionMatrix() = 0;


protected:
    LinearStateModel() noexcept = default;

    LinearStateModel(const LinearStateModel& state_model) noexcept = delete;

    LinearStateModel& operator=(const LinearStateModel& state_model) noexcept = delete;

    LinearStateModel(LinearStateModel&& state_model) noexcept = default;

    LinearStateModel& operator=(LinearStateModel&& state_model) noexcept = default;
};

#endif /* LINEARSTATEMODEL_H */
