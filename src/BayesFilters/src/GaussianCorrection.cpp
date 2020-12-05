/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/GaussianCorrection.h>

#include <exception>

using namespace bfl;
using namespace Eigen;


void GaussianCorrection::correct(const GaussianMixture& pred_state, GaussianMixture& corr_state)
{
    /* Perform correction if required and if measurements can be frozen. */
    if (!skip_)
        correctStep(pred_state, corr_state);
    else
        corr_state = pred_state;
}


bool GaussianCorrection::skip(const bool status)
{
    skip_ = status;

    return true;
}


bool GaussianCorrection::freeze_measurements()
{
    return getMeasurementModel().freeze();
}


std::pair<bool, VectorXd> GaussianCorrection::getLikelihood()
{
    throw std::runtime_error("ERROR::GAUSSIANCORRECTION::GETLIKELIHOOD\nERROR:\n\tCall to unimplemented base class method.");
}
