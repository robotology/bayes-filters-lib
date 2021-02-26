/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include "BayesFilters/PFPrediction.h"

#include <exception>
#include <iostream>

using namespace bfl;
using namespace Eigen;


void PFPrediction::predict(const ParticleSet& prev_particles, ParticleSet& pred_particles)
{
    if (!skip_)
        predictStep(prev_particles, pred_particles);
    else
        pred_particles = prev_particles;
}


bool PFPrediction::skip(const std::string& what_step, const bool status)
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

        skip_ = getStateModel().is_skipping() & getStateModel().exogenous_model().is_skipping();
    }
    else if (what_step == "exogenous")
    {
        getStateModel().skip("exogenous", status);

        skip_ = getStateModel().is_skipping() & getStateModel().exogenous_model().is_skipping();
    }
    else
        return false;

    return true;
}


bool PFPrediction::is_skipping()
{
    return skip_;
}
