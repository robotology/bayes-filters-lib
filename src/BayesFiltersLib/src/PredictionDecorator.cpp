#include <utility>

#include "BayesFiltersLib/PredictionDecorator.h"

using namespace Eigen;

namespace bfl
{
    
PredictionDecorator::PredictionDecorator(std::unique_ptr<Prediction> prediction) noexcept :
    prediction_(std::move(prediction)) { }


PredictionDecorator::~PredictionDecorator() noexcept { }


PredictionDecorator::PredictionDecorator(PredictionDecorator&& prediction) noexcept :
    prediction_(std::move(prediction.prediction_)) { }


PredictionDecorator& PredictionDecorator::operator=(PredictionDecorator&& prediction) noexcept
{
    prediction_ = std::move(prediction.prediction_);

    return *this;
}


void PredictionDecorator::predict(const Ref<const VectorXf>& prev_state, Ref<VectorXf> pred_state)
{
    prediction_->predict(prev_state, pred_state);
}

} // namespace bfl
