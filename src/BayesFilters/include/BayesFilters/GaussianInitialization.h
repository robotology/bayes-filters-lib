/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef GAUSSIANINITIALIZATION_H
#define GAUSSIANINITIALIZATION_H

#include <BayesFilters/Gaussian.h>

#include <Eigen/Dense>

namespace bfl {
    class GaussianInitialization;
}


class bfl::GaussianInitialization
{
public:
    virtual ~GaussianInitialization() noexcept = default;

    virtual void initialize(Gaussian& state) = 0;
};

#endif /* PARTICLESETINITIALIZATION_H */
