/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/StateModel.h>

using namespace bfl;
using namespace Eigen;


bool StateModel::skip(const std::string& what_step, const bool status)
{
    if (what_step == "state")
        skip_ = status;
    else if (what_step == "exogenous")
        exogenous_model().skip(what_step, status);
    else
        return false;

    return true;
}


bool StateModel::getSkipState()
{
    return skip_;
}


bool StateModel::add_exogenous_model(std::unique_ptr<ExogenousModel> exogenous_model)
{
    exogenous_model_ = std::move(exogenous_model);

    return true;
}


bool StateModel::have_exogenous_model() noexcept
{
    if (exogenous_model_)
        return true;
    else
        return false;
}


ExogenousModel& StateModel::exogenous_model()
{
    if (exogenous_model_)
        return *exogenous_model_;

    throw std::runtime_error("ERROR::LTISTATEMODEL::GET_EXOGENOUS_MODEL\nERROR:\n\tNo valid ExogenousModel object present in LTIStateModel object. Use LTIStateModel::add_exogenous_model() to add one.");
}



Eigen::MatrixXd StateModel::getJacobian()
{
    throw std::runtime_error("ERROR::STATEMODEL::GETJACOBIAN\nERROR:\n\tMethod not implemented.");
}


Eigen::VectorXd StateModel::getTransitionProbability(const Ref<const MatrixXd>& prev_states, const Ref<const MatrixXd>& cur_states)
{
    throw std::runtime_error("ERROR::STATEMODEL::TRANSITIONPROBABILITY\nERROR:\n\tMethod not implemented.");
}


Eigen::MatrixXd StateModel::getNoiseCovarianceMatrix()
{
    throw std::runtime_error("ERROR::STATEMODEL::GETNOISECOVARIANCEMATRIX\nERROR:\n\tMethod not implemented.");
}


Eigen::MatrixXd StateModel::getNoiseSample(const std::size_t num)
{
    throw std::runtime_error("ERROR::STATEMODEL::GETNOISESAMPLE\nERROR:\n\tMethod not implemented.");
}
