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


BootstrapCorrection::BootstrapCorrection(std::unique_ptr<MeasurementModel> measurement_model, std::unique_ptr<LikelihoodModel> likelihood_model) noexcept :
    measurement_model_(std::move(measurement_model)),
    likelihood_model_(std::move(likelihood_model))
{ }


BootstrapCorrection::BootstrapCorrection(BootstrapCorrection&& correction) noexcept :
    PFCorrection(std::move(correction)),
    measurement_model_(std::move(correction.measurement_model_)),
    likelihood_model_(std::move(correction.likelihood_model_))
{ }


BootstrapCorrection& BootstrapCorrection::operator=(BootstrapCorrection&& correction) noexcept
{
    PFCorrection::operator=(std::move(correction));

    return *this;
}


MeasurementModel& BootstrapCorrection::getMeasurementModel() noexcept
{
    return *measurement_model_;
}


LikelihoodModel& BootstrapCorrection::getLikelihoodModel() noexcept
{
    return *likelihood_model_;
}


void BootstrapCorrection::correctStep(const ParticleSet& pred_particles, ParticleSet& cor_particles)
{
    std::tie(valid_likelihood_, likelihood_) = getLikelihoodModel().likelihood(getMeasurementModel(), pred_particles.state());

    cor_particles = pred_particles;

    if (valid_likelihood_)
        cor_particles.weight() += (likelihood_.array() + std::numeric_limits<double>::min()).log().matrix();
}


std::pair<bool, VectorXd> BootstrapCorrection::getLikelihood()
{
    return std::make_pair(valid_likelihood_, likelihood_);
}
