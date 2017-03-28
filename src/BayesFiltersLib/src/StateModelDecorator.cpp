#include "BayesFiltersLib/StateModelDecorator.h"

using namespace Eigen;


namespace bfl
{

void StateModelDecorator::propagate(const Ref<const VectorXf>& cur_state, Ref<VectorXf> prop_state)
{
    prediction_->propagate(cur_state, prop_state);
}


void StateModelDecorator::noiseSample(Ref<VectorXf> sample)
{
    prediction_->noiseSample(sample);
}


void StateModelDecorator::motion(const Ref<const VectorXf>& prev_state, Ref<VectorXf> next_state)
{
    prediction_->motion(prev_state, next_state);
}


StateModelDecorator::StateModelDecorator(std::unique_ptr<StateModel> prediction) noexcept :
    prediction_(std::move(prediction)) { }


StateModelDecorator::~StateModelDecorator() noexcept { }


StateModelDecorator::StateModelDecorator(StateModelDecorator&& prediction) noexcept :
    prediction_(std::move(prediction.prediction_)) { }


StateModelDecorator& StateModelDecorator::operator=(StateModelDecorator&& prediction) noexcept
{
    prediction_ = std::move(prediction.prediction_);

    return *this;
}

}
