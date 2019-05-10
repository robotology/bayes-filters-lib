/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/BootstrapCorrection.h>

#include <cmath>
#include <utility>

using namespace bfl;
using namespace Eigen;


void BootstrapCorrection::correctStep(const ParticleSet& pred_particles, ParticleSet& cor_particles)
{
    std::tie(valid_likelihood_, likelihood_) = likelihood_model_->likelihood(*measurement_model_, pred_particles.state());

    cor_particles = pred_particles;

    if (valid_likelihood_)
        cor_particles.weight() += (likelihood_.array() + std::numeric_limits<double>::min()).log().matrix();
}


LikelihoodModel& BootstrapCorrection::getLikelihoodModel()
{
    return *likelihood_model_;
}


MeasurementModel& BootstrapCorrection::getMeasurementModel()
{
    return *measurement_model_;
}


std::pair<bool, VectorXd> BootstrapCorrection::getLikelihood()
{
    return std::make_pair(valid_likelihood_, likelihood_);
}


void BootstrapCorrection::setLikelihoodModel(std::unique_ptr<LikelihoodModel> likelihood_model)
{
    likelihood_model_ = std::move(likelihood_model);
}


void BootstrapCorrection::setMeasurementModel(std::unique_ptr<MeasurementModel> measurement_model)
{
    measurement_model_ = std::move(measurement_model);
}
