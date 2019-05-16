/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef GAUSSIANPREDICTION_H
#define GAUSSIANPREDICTION_H

#include <BayesFilters/ExogenousModel.h>
#include <BayesFilters/GaussianMixturePrediction.h>
#include <BayesFilters/Skippable.h>
#include <BayesFilters/StateModel.h>

#include <Eigen/Dense>

namespace bfl {
    class GaussianPrediction;
}


class bfl::GaussianPrediction : public GaussianMixturePrediction, public Skippable
{
public:
    virtual ~GaussianPrediction() noexcept = default;

    void predict(const GaussianMixture& prev_state, GaussianMixture& pred_state) override;

    bool skip(const std::string& what_step, const bool status) override;

    bool is_skipping() override;

    virtual StateModel& getStateModel() noexcept = 0;


protected:
    GaussianPrediction() noexcept = default;

    GaussianPrediction(const GaussianPrediction& prediction) noexcept = delete;

    GaussianPrediction& operator=(const GaussianPrediction& prediction) noexcept = delete;

    GaussianPrediction(GaussianPrediction&& prediction) noexcept = default;

    GaussianPrediction& operator=(GaussianPrediction&& prediction) noexcept = default;

    virtual void predictStep(const GaussianMixture& prev_state, GaussianMixture& pred_state) = 0;


private:
    bool skip_ = false;
};

#endif /* GAUSSIANPREDICTION_H */
