/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef LINEARSTATEMODEL_H
#define LINEARSTATEMODEL_H

#include <Eigen/Dense>

#include <BayesFilters/AdditiveStateModel.h>

namespace bfl {
    class LinearStateModel;
}


class bfl::LinearStateModel : public bfl::AdditiveStateModel
{
public:
    virtual ~LinearStateModel() noexcept = default;

    virtual void propagate(const Eigen::Ref<const Eigen::MatrixXd>& cur_states, Eigen::Ref<Eigen::MatrixXd> mot_states) override;

    virtual Eigen::MatrixXd getStateTransitionMatrix() = 0;
};

#endif /* LINEARSTATEMODEL_H */
