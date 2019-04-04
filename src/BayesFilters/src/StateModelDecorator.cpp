/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/StateModelDecorator.h>

using namespace bfl;
using namespace Eigen;


StateModelDecorator::StateModelDecorator(std::unique_ptr<StateModel> state_model) noexcept :
    state_model_(std::move(state_model)) { }


StateModelDecorator::StateModelDecorator(StateModelDecorator&& state_model) noexcept :
    state_model_(std::move(state_model.state_model_)) { }


StateModelDecorator::~StateModelDecorator() noexcept { }


StateModelDecorator& StateModelDecorator::operator=(StateModelDecorator&& state_model) noexcept
{
    state_model_ = std::move(state_model.state_model_);

    return *this;
}


void StateModelDecorator::propagate(const Ref<const MatrixXd>& cur_states, Ref<MatrixXd> prop_states)
{
    state_model_->propagate(cur_states, prop_states);
}


void StateModelDecorator::motion(const Ref<const MatrixXd>& cur_states, Ref<MatrixXd> mot_states)
{
    state_model_->motion(cur_states, mot_states);
}


bool StateModelDecorator::setProperty(const std::string& property)
{
    return state_model_->setProperty(property);
}


MatrixXd StateModelDecorator::getNoiseCovarianceMatrix()
{
    return state_model_->getNoiseCovarianceMatrix();
}


MatrixXd StateModelDecorator::getNoiseSample(const std::size_t num)
{
    return state_model_->getNoiseSample(num);
}


std::pair<std::size_t, std::size_t> StateModelDecorator::getOutputSize() const
{
    return state_model_->getOutputSize();
}
