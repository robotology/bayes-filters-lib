#include <utility>

#include <FilteringFunction/PredictionDecorator.h>


PredictionDecorator::PredictionDecorator(std::shared_ptr<Prediction> prediction) noexcept :
    prediction_(prediction) { }


PredictionDecorator::~PredictionDecorator() noexcept { }


PredictionDecorator::PredictionDecorator(const PredictionDecorator& prediction) :
    prediction_(prediction.prediction_) { }


PredictionDecorator::PredictionDecorator(PredictionDecorator&& prediction) noexcept :
    prediction_(std::move(prediction.prediction_)) { }


PredictionDecorator& PredictionDecorator::operator=(const PredictionDecorator& prediction)
{
    PredictionDecorator tmp(prediction);
    *this = std::move(tmp);
    
    return *this;
}


PredictionDecorator& PredictionDecorator::operator=(PredictionDecorator&& prediction) noexcept
{
    prediction_ = std::move(prediction.prediction_);

    return *this;
}


void PredictionDecorator::predict(const Eigen::Ref<const Eigen::VectorXf> & prev_state, Eigen::Ref<Eigen::VectorXf> pred_state)
{
    prediction_->predict(prev_state, pred_state);
}
