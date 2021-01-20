/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef UKFPREDICTION_H
#define UKFPREDICTION_H

#include <BayesFilters/AdditiveStateModel.h>
#include <BayesFilters/ExogenousModel.h>
#include <BayesFilters/GaussianMixture.h>
#include <BayesFilters/GaussianPrediction.h>
#include <BayesFilters/sigma_point.h>
#include <BayesFilters/StateModel.h>

#include <memory>

namespace bfl {
    class UKFPrediction;
}


class bfl::UKFPrediction : public GaussianPrediction
{
public:
    enum class UKFPredictionType
    {
        Generic,
        Additive
    };

    UKFPrediction(std::unique_ptr<StateModel> state_model, const size_t n, const double alpha, const double beta, const double kappa) noexcept;

    UKFPrediction(std::unique_ptr<AdditiveStateModel> state_model, const size_t n, const double alpha, const double beta, const double kappa) noexcept;

    UKFPrediction(const UKFPrediction& prediction) noexcept = delete;

    UKFPrediction& operator=(const UKFPrediction& prediction) noexcept = delete;

    UKFPrediction(UKFPrediction&& prediction) noexcept;

    UKFPrediction& operator=(UKFPrediction&& prediction) noexcept;

    virtual ~UKFPrediction() noexcept = default;

    StateModel& getStateModel() noexcept override;


protected:
    void predictStep(const GaussianMixture& prev_state, GaussianMixture& pred_state) override;


private:
    std::unique_ptr<StateModel> state_model_;

    std::unique_ptr<AdditiveStateModel> add_state_model_;

    /**
     * Distinguish between a UKFPrediction using a generic StateModel
     * and a UKFPrediction using an AdditiveStateModel.
     */
    UKFPredictionType type_;

    /**
     * Unscented transform weight.
     */
    sigma_point::UTWeight ut_weight_;
};

#endif /* UKFPREDICTION_H */
