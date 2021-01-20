/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/UKFPrediction.h>

using namespace bfl;
using namespace bfl::sigma_point;
using namespace Eigen;


UKFPrediction::UKFPrediction
(
    std::unique_ptr<StateModel> state_model,
    const size_t n,
    const double alpha,
    const double beta,
    const double kappa
) noexcept :
    state_model_(std::move(state_model)),
    type_(UKFPredictionType::Generic),
    ut_weight_(n, alpha, beta, kappa)
{ }


UKFPrediction::UKFPrediction
(
    std::unique_ptr<AdditiveStateModel> state_model,
    const size_t n,
    const double alpha,
    const double beta,
    const double kappa
) noexcept :
    add_state_model_(std::move(state_model)),
    type_(UKFPredictionType::Additive),
    ut_weight_(n, alpha, beta, kappa)
{ }


UKFPrediction::UKFPrediction(UKFPrediction&& prediction) noexcept:
    GaussianPrediction(std::move(prediction)),
    state_model_(std::move(prediction.state_model_)),
    add_state_model_(std::move(prediction.add_state_model_)),
    type_(prediction.type_),
    ut_weight_(std::move(prediction.ut_weight_))
{ }


UKFPrediction& UKFPrediction::operator=(UKFPrediction&& prediction) noexcept
{
    if (this == &prediction)
        return *this;

    GaussianPrediction::operator=(std::move(prediction));

    state_model_ = std::move(prediction.state_model_);

    add_state_model_ = std::move(prediction.add_state_model_);

    type_ = prediction.type_;

    ut_weight_ = std::move(prediction.ut_weight_);

    return *this;
}


bfl::StateModel& UKFPrediction::getStateModel() noexcept
{
    if (type_ == UKFPredictionType::Additive)
        return *add_state_model_;

    return *state_model_;
}


void UKFPrediction::predictStep(const GaussianMixture& prev_state, GaussianMixture& pred_state)
{
    if (getStateModel().getSkipState())
    {
        pred_state = prev_state;

        return;
    }


    /* Evaluate predicted mean and predicted covariance using unscented transform. */
    if (type_ == UKFPredictionType::Generic)
    {
        /* Augment the previous state using process noise statistics. */
        GaussianMixture prev_state_augmented = prev_state;
        prev_state_augmented.augmentWithNoise(state_model_->getNoiseCovarianceMatrix());

        std::tie(pred_state, std::ignore) = unscented_transform(prev_state_augmented, ut_weight_, *state_model_);
    }
    else if (type_ == UKFPredictionType::Additive)
    {
        std::tie(pred_state, std::ignore) = unscented_transform(prev_state, ut_weight_, *add_state_model_);
    }
}
