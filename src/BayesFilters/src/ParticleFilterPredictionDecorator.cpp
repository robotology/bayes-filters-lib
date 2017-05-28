#include <utility>

#include "BayesFilters/ParticleFilterPredictionDecorator.h"

using namespace bfl;
using namespace Eigen;

    
ParticleFilterPredictionDecorator::ParticleFilterPredictionDecorator(std::unique_ptr<ParticleFilterPrediction> prediction) noexcept :
    prediction_(std::move(prediction)) { }


ParticleFilterPredictionDecorator::~ParticleFilterPredictionDecorator() noexcept { }


ParticleFilterPredictionDecorator::ParticleFilterPredictionDecorator(ParticleFilterPredictionDecorator&& prediction) noexcept :
    prediction_(std::move(prediction.prediction_)) { }


ParticleFilterPredictionDecorator& ParticleFilterPredictionDecorator::operator=(ParticleFilterPredictionDecorator&& prediction) noexcept
{
    prediction_ = std::move(prediction.prediction_);

    return *this;
}


void ParticleFilterPredictionDecorator::predict(const Ref<const VectorXf>& prev_state, Ref<VectorXf> pred_state)
{
    prediction_->predict(prev_state, pred_state);
}


void ParticleFilterPredictionDecorator::motion(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> prop_state)
{
    prediction_->motion(cur_state, prop_state);
}


void ParticleFilterPredictionDecorator::motionDisturbance(Eigen::Ref<Eigen::VectorXf> sample)
{
    prediction_->motionDisturbance(sample);
}


bool ParticleFilterPredictionDecorator::setStateModelProperty(const std::string& property)
{
    return prediction_->setStateModelProperty(property);
}
