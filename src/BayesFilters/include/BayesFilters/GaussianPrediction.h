/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef GAUSSIANPREDICTION_H
#define GAUSSIANPREDICTION_H

#include <BayesFilters/ExogenousModel.h>
#include <BayesFilters/GaussianMixture.h>
#include <BayesFilters/StateModel.h>

#include <Eigen/Dense>

namespace bfl {
    class GaussianPrediction;
}


class bfl::GaussianPrediction
{
public:
    virtual ~GaussianPrediction() noexcept = default;

    void predict(const GaussianMixture& prev_state, GaussianMixture& pred_state);

    bool skip(const std::string& what_step, const bool status);

    bool getSkipState();

    bool getSkipExogenous();

    virtual StateModel& getStateModel() noexcept = 0;

    virtual ExogenousModel& getExogenousModel();


protected:
    virtual void predictStep(const GaussianMixture& prev_state, GaussianMixture& pred_state) = 0;


private:
    bool skip_prediction_ = false;

    bool skip_state_ = false;

    bool skip_exogenous_ = false;
};

#endif /* GAUSSIANPREDICTION_H */
