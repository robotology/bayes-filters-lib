#include "BayesFiltersLib/StateModelDecorator.h"

using namespace Eigen;


namespace bfl
{

void StateModelDecorator::propagate(const Ref<const VectorXf>& cur_state, Ref<VectorXf> prop_state)
{
    state_model_->propagate(cur_state, prop_state);
}


void StateModelDecorator::noiseSample(Ref<VectorXf> sample)
{
    state_model_->noiseSample(sample);
}


bool StateModelDecorator::setProperty(const std::string& property)
{
    return state_model_->setProperty(property);
}


StateModelDecorator::StateModelDecorator(std::unique_ptr<StateModel> state_model) noexcept :
    state_model_(std::move(state_model)) { }


StateModelDecorator::~StateModelDecorator() noexcept { }


StateModelDecorator::StateModelDecorator(StateModelDecorator&& state_model) noexcept :
    state_model_(std::move(state_model.state_model_)) { }


StateModelDecorator& StateModelDecorator::operator=(StateModelDecorator&& state_model) noexcept
{
    state_model_ = std::move(state_model.state_model_);

    return *this;
}

}
