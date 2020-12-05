/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef KFPREDICTION_H
#define KFPREDICTION_H

#include <BayesFilters/GaussianMixture.h>
#include <BayesFilters/GaussianPrediction.h>
#include <BayesFilters/LinearStateModel.h>
#include <BayesFilters/ExogenousModel.h>

#include <memory>

namespace bfl {
    class KFPrediction;
}


class bfl::KFPrediction : public bfl::GaussianPrediction
{
public:
    KFPrediction(std::unique_ptr<LinearStateModel> state_model) noexcept;

    KFPrediction(std::unique_ptr<LinearStateModel> state_model, std::unique_ptr<ExogenousModel> exogenous_model) noexcept;

    KFPrediction(KFPrediction&& kf_prediction) noexcept;

    virtual ~KFPrediction() noexcept = default;

    StateModel& getStateModel() noexcept override;

    ExogenousModel& getExogenousModel() override;

protected:
    void predictStep(const GaussianMixture& prev_state, GaussianMixture& pred_state) override;

    std::unique_ptr<LinearStateModel> state_model_;

    std::unique_ptr<ExogenousModel> exogenous_model_;
};

#endif /* KFPREDICTION_H */
