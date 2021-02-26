/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/KFPrediction.h>

using namespace bfl;
using namespace Eigen;


KFPrediction::KFPrediction(std::unique_ptr<LinearStateModel> state_model) noexcept :
    state_model_(std::move(state_model))
{ }


KFPrediction::KFPrediction(KFPrediction&& prediction) noexcept:
    GaussianPrediction(std::move(prediction)),
    state_model_(std::move(prediction.state_model_))
{ }


KFPrediction& KFPrediction::operator=(KFPrediction&& prediction) noexcept
{
    if (this == &prediction)
        return *this;

    GaussianPrediction::operator=(std::move(prediction));

    state_model_ = std::move(prediction.state_model_);

    return *this;
}


bfl::StateModel& KFPrediction::getStateModel() noexcept
{
    return *state_model_;
}


void KFPrediction::predictStep(const GaussianMixture& prev_state, GaussianMixture& pred_state)
{
    if (getStateModel().is_skipping())
    {
        pred_state = prev_state;

        return;
    }

    /* Evaluate predicted mean
       x_{k+1} = F_{k} x_{k}   */
    getStateModel().propagate(prev_state.mean(), pred_state.mean());

    /* Evaluate predicted covariance.
       P_{k+1} = F_{k} * P_{k} * F_{k}' + Q */
    MatrixXd F = state_model_->getStateTransitionMatrix();

    for (size_t i=0; i < prev_state.components; i++)
        pred_state.covariance(i).noalias() = F * prev_state.covariance(i) * F.transpose() + state_model_->getNoiseCovarianceMatrix();
}
