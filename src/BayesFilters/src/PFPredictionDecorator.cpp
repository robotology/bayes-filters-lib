#include "BayesFilters/PFPredictionDecorator.h"

#include <utility>

using namespace bfl;
using namespace Eigen;

    
PFPredictionDecorator::PFPredictionDecorator(std::unique_ptr<PFPrediction> prediction) noexcept :
    prediction_(std::move(prediction)) { }


PFPredictionDecorator::~PFPredictionDecorator() noexcept { }


PFPredictionDecorator::PFPredictionDecorator(PFPredictionDecorator&& prediction) noexcept :
    prediction_(std::move(prediction.prediction_)) { }


PFPredictionDecorator& PFPredictionDecorator::operator=(PFPredictionDecorator&& prediction) noexcept
{
    prediction_ = std::move(prediction.prediction_);

    return *this;
}


void PFPredictionDecorator::predict(const Ref<const VectorXf>& prev_state, Ref<VectorXf> pred_state)
{
    prediction_->predict(prev_state, pred_state);
}
