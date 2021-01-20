/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef GAUSSIANMIXTUREPREDICTION_H
#define GAUSSIANMIXTUREPREDICTION_H

#include <BayesFilters/GaussianMixture.h>

#include <Eigen/Dense>

namespace bfl {
    class GaussianMixturePrediction;
}


class bfl::GaussianMixturePrediction
{
public:
    virtual void predict(const GaussianMixture& prev_state, GaussianMixture& pred_state) = 0;
};

#endif /* GAUSSIANMIXTUREPREDICTION_H */
