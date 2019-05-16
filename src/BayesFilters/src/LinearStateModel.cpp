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


void LinearStateModel::propagate(const Eigen::Ref<const Eigen::MatrixXd>& cur_states, Eigen::Ref<Eigen::MatrixXd> prop_states)
{
    if (getSkipState() && (have_exogenous_model() && exogenous_model().getSkipState()))
    {
        prop_states = cur_states;
    }
    else if (!getSkipState() && (have_exogenous_model() && !exogenous_model().getSkipState()))
    {
        MatrixXd exogenous_state(cur_states.rows(), cur_states.cols());

        exogenous_model().propagate(cur_states, exogenous_state);

        prop_states = getStateTransitionMatrix() * cur_states + exogenous_state;
    }
    else if (!getSkipState())
    {
        prop_states = getStateTransitionMatrix() * cur_states;
    }
    else if (have_exogenous_model() && !exogenous_model().getSkipState())
    {
        exogenous_model().propagate(cur_states, prop_states);
    }
}
