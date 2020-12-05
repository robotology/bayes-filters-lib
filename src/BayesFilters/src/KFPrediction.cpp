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


KFPrediction::KFPrediction(std::unique_ptr<LinearStateModel> state_model, std::unique_ptr<ExogenousModel> exogenous_model) noexcept :
    state_model_(std::move(state_model)),
    exogenous_model_(std::move(exogenous_model))
{ }


KFPrediction::KFPrediction(KFPrediction&& kf_prediction) noexcept:
    state_model_(std::move(kf_prediction.state_model_)),
    exogenous_model_(std::move(kf_prediction.exogenous_model_))
{ }


bfl::StateModel& KFPrediction::getStateModel() noexcept
{
    return *state_model_;
}


bfl::ExogenousModel& KFPrediction::getExogenousModel()
{
    if (exogenous_model_ == nullptr)
        throw std::runtime_error("ERROR::KFPREDICTION::GETEXOGENOUSMODEL\nERROR:\n\tExogenous model does not exist. Did you pass it to KFPrediction ctor?");

    return *exogenous_model_;
}


void KFPrediction::predictStep(const GaussianMixture& prev_state, GaussianMixture& pred_state)
{
    bool skip_exogenous = getSkipExogenous() || (exogenous_model_ == nullptr);

    if (getSkipState() && skip_exogenous)
    {
        /* Skip prediction step entirely. */
        pred_state = prev_state;
        return;
    }

    if (!getSkipState())
    {
        /* Evaluate predicted mean
           x_{k+1} = F_{k} x_{k}   */
        MatrixXd F = state_model_->getStateTransitionMatrix();
        pred_state.mean().noalias() = F * prev_state.mean();

        /* Evaluate predicted covariance.
           P_{k+1} = F_{k} * P_{k} * F_{k}' + Q */
        for (size_t i=0; i < prev_state.components; i++)
            pred_state.covariance(i).noalias() = F * prev_state.covariance(i) * F.transpose() + state_model_->getNoiseCovarianceMatrix();

        if (!skip_exogenous)
        {
            /* Since it is not clear whether ExogenousModel::propagate takes
               into account aliasing or not, then a temporary is used here. */
            MatrixXd tmp(pred_state.mean().rows(), pred_state.mean().cols());

            exogenous_model_->propagate(pred_state.mean(), tmp);

            pred_state.mean() = std::move(tmp);
        }
    }
    else
    {
        /* Assuming that also the uncertainty due to the noise is neglected if (getSkipState == true). */
        pred_state.covariance() = prev_state.covariance();

        if (!skip_exogenous)
        {
            exogenous_model_->propagate(prev_state.mean(), pred_state.mean());
        }
    }
}
