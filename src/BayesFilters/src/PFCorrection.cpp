/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/PFCorrection.h>

using namespace bfl;
using namespace Eigen;


PFCorrection::PFCorrection() noexcept { };


void PFCorrection::correct(const ParticleSet& pred_particles, ParticleSet& cor_particles)
{
    /* Perform correction if required and if measurements can be frozen. */
    if (!skip_)
        correctStep(pred_particles, cor_particles);
    else
        cor_particles = pred_particles;
}


bool PFCorrection::skip(const bool status)
{
    skip_ = status;

    return true;
}


bool PFCorrection::freeze_measurements()
{
    return getMeasurementModel().freeze();
}
