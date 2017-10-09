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
