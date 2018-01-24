#include "BayesFilters/StateModelDecorator.h"

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


void StateModelDecorator::propagate(const Ref<const MatrixXf>& cur_states, Ref<MatrixXf> prop_states)
{
    state_model_->propagate(cur_states, prop_states);
}


void StateModelDecorator::motion(const Ref<const MatrixXf>& cur_states, Ref<MatrixXf> mot_states)
{
    state_model_->motion(cur_states, mot_states);
}


MatrixXf StateModelDecorator::getNoiseSample(const int num)
{
    return state_model_->getNoiseSample(num);
}


MatrixXf StateModelDecorator::getNoiseCovarianceMatrix()
{
    return state_model_->getNoiseCovarianceMatrix();
}


bool StateModelDecorator::setProperty(const std::string& property)
{
    return state_model_->setProperty(property);
}
