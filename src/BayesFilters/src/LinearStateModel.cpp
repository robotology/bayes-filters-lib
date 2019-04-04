/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/LinearStateModel.h>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


void LinearStateModel::propagate(const Eigen::Ref<const Eigen::MatrixXd>& cur_states, Eigen::Ref<Eigen::MatrixXd> pred_states)
{
    pred_states = getStateTransitionMatrix() * cur_states;
}
