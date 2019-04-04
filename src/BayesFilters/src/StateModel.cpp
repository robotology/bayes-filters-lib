/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/StateModel.h>

using namespace bfl;
using namespace Eigen;


Eigen::MatrixXd StateModel::getJacobian()
{
    throw std::runtime_error("ERROR::STATEMODEL::GETJACOBIAN\nERROR:\n\tMethod not implemented.");
}


Eigen::VectorXd StateModel::getTransitionProbability(const Eigen::Ref<const Eigen::MatrixXd>& prev_states, Eigen::Ref<Eigen::MatrixXd> cur_states)
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
