#include <utility>

#include "BayesFiltersLib/PredictionDecorator.h"

using namespace bfl;
using namespace Eigen;

    
PredictionDecorator::PredictionDecorator(std::unique_ptr<ParticleFilterPrediction> prediction) noexcept :
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


void PredictionDecorator::motion(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> prop_state)
{
    prediction_->motion(cur_state, prop_state);
}


void PredictionDecorator::motionDisturbance(Eigen::Ref<Eigen::VectorXf> sample)
{
    prediction_->motionDisturbance(sample);
}


bool PredictionDecorator::setMotionModelProperty(const std::string& property)
{
    return prediction_->setMotionModelProperty(property);
}
