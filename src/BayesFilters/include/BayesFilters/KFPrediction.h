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

    KFPrediction(const KFPrediction& prediction) noexcept = delete;

    KFPrediction& operator=(const KFPrediction& prediction) noexcept = delete;

    KFPrediction(KFPrediction&& prediction) noexcept;

    KFPrediction& operator=(KFPrediction&& prediction) noexcept;

    virtual ~KFPrediction() noexcept = default;

    StateModel& getStateModel() noexcept override;


protected:
    void predictStep(const GaussianMixture& prev_state, GaussianMixture& pred_state) override;


private:
    std::unique_ptr<LinearStateModel> state_model_;
};

#endif /* KFPREDICTION_H */
