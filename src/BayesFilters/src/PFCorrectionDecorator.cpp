/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/PFCorrectionDecorator.h>

#include <utility>

using namespace bfl;
using namespace Eigen;


PFCorrectionDecorator::PFCorrectionDecorator(std::unique_ptr<PFCorrection> correction) noexcept :
    correction_(std::move(correction)) { }


PFCorrectionDecorator::PFCorrectionDecorator(PFCorrectionDecorator&& correction) noexcept :
    correction_(std::move(correction.correction_)) { }


PFCorrectionDecorator::~PFCorrectionDecorator() noexcept { }


void PFCorrectionDecorator::setLikelihoodModel(std::unique_ptr<LikelihoodModel> measurement_model)
{
    correction_->setLikelihoodModel(std::move(measurement_model));
}


void PFCorrectionDecorator::setMeasurementModel(std::unique_ptr<MeasurementModel> observation_model)
{
    correction_->setMeasurementModel(std::move(observation_model));
}


MeasurementModel& PFCorrectionDecorator::getMeasurementModel()
{
    return correction_->getMeasurementModel();
}


std::pair<bool, VectorXd> PFCorrectionDecorator::getLikelihood()
{
    return correction_->getLikelihood();
}


LikelihoodModel& PFCorrectionDecorator::getLikelihoodModel()
{
    return correction_->getLikelihoodModel();
}


void PFCorrectionDecorator::correctStep(const ParticleSet& pred_particles, ParticleSet& cor_particles)
{
    correction_->correctStep(pred_particles, cor_particles);
}
