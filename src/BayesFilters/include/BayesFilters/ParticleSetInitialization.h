/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef PARTICLESETINITIALIZATION_H
#define PARTICLESETINITIALIZATION_H

#include <BayesFilters/ParticleSet.h>

#include <Eigen/Dense>

namespace bfl {
    class ParticleSetInitialization;
}


class bfl::ParticleSetInitialization
{
public:
    virtual ~ParticleSetInitialization() noexcept = default;

    virtual bool initialize(ParticleSet& particles) = 0;
};

#endif /* PARTICLESETINITIALIZATION_H */
