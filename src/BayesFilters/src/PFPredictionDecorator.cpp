/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/PFPredictionDecorator.h>

#include <utility>

using namespace bfl;
using namespace Eigen;


PFPredictionDecorator::PFPredictionDecorator(std::unique_ptr<PFPrediction> prediction) noexcept :
    prediction_(std::move(prediction)) { }


PFPredictionDecorator::PFPredictionDecorator(PFPredictionDecorator&& prediction) noexcept :
    prediction_(std::move(prediction.prediction_)) { }


PFPredictionDecorator::~PFPredictionDecorator() noexcept { }


PFPredictionDecorator& PFPredictionDecorator::operator=(PFPredictionDecorator&& prediction) noexcept
{
    prediction_ = std::move(prediction.prediction_);

    return *this;
}


void PFPredictionDecorator::predictStep(const ParticleSet& prev_particles, ParticleSet& pred_particles)
{
    prediction_->predictStep(prev_particles, pred_particles);
}


StateModel& PFPredictionDecorator::getStateModel()
{
    return prediction_->getStateModel();
}


void PFPredictionDecorator::setStateModel(std::unique_ptr<StateModel> state_model)
{
    prediction_->setStateModel(std::move(state_model));
}


ExogenousModel& PFPredictionDecorator::getExogenousModel()
{
    return prediction_->getExogenousModel();
}


void PFPredictionDecorator::setExogenousModel(std::unique_ptr<ExogenousModel> exogenous_model)
{
    prediction_->setExogenousModel(std::move(exogenous_model));
}
