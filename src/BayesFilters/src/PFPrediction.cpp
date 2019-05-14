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
    if (!skip_prediction_)
        predictStep(prev_particles, pred_particles);
    else
        pred_particles = prev_particles;
}


bool PFPrediction::skip(const std::string& what_step, const bool status)
{
    if (what_step == "prediction")
    {
        skip_prediction_ = status;

        skip_state_ = status;
        skip_exogenous_ = status;
    }
    else if (what_step == "state")
    {
        skip_state_ = status;

        skip_prediction_ = skip_state_ & skip_exogenous_;
    }
    else if (what_step == "exogenous")
    {
        skip_exogenous_ = status;

        skip_prediction_ = skip_state_ & skip_exogenous_;
    }
        return false;

    return true;
}


bool PFPrediction::getSkipState()
{
    return skip_state_;
}


bool PFPrediction::getSkipExogenous()
{
    return skip_exogenous_;
}


ExogenousModel& PFPrediction::getExogenousModel()
{
    throw std::runtime_error("ERROR::PFPREDICTION::GETEXOGENOUSMODEL\nERROR:\n\tObject class has no valid ExogenousModel object.");
}
