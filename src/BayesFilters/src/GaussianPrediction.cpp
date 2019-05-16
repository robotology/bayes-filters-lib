/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/GaussianPrediction.h>

#include <exception>
#include <iostream>

using namespace bfl;
using namespace Eigen;



void GaussianPrediction::predict(const GaussianMixture& prev_state, GaussianMixture& pred_state)
{
    if (!skip_)
        predictStep(prev_state, pred_state);
    else
        pred_state = prev_state;
}


bool GaussianPrediction::skip(const std::string& what_step, const bool status)
{
    if (what_step == "prediction")
    {
        skip_ = status;

        getStateModel().skip("state", status);

        getStateModel().skip("exogenous", status);
    }
    else if (what_step == "state")
    {
        getStateModel().skip("state", status);

        skip_ = getStateModel().getSkipState() & getStateModel().exogenous_model().getSkipState();
    }
    else if (what_step == "exogenous")
    {
        getStateModel().skip("exogenous", status);

        skip_ = getStateModel().getSkipState() & getStateModel().exogenous_model().getSkipState();
    }
    else
        return false;

    return true;
}


bool GaussianPrediction::getSkipState()
{
    return skip_;
}
