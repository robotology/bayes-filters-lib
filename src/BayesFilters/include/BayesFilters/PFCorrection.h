/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef PFCORRECTION_H
#define PFCORRECTION_H

#include <BayesFilters/LikelihoodModel.h>
#include <BayesFilters/MeasurementModel.h>
#include <BayesFilters/ParticleSet.h>

#include <memory>
#include <utility>

#include <Eigen/Dense>

namespace bfl {
    class PFCorrection;
}


class bfl::PFCorrection
{
public:
    virtual ~PFCorrection() noexcept { };

    void correct(const ParticleSet& pred_particles, ParticleSet& cor_particles);

    bool skip(const bool status);

    virtual void setLikelihoodModel(std::unique_ptr<LikelihoodModel> observation_model) = 0;

    virtual void setMeasurementModel(std::unique_ptr<MeasurementModel> measurement_model) = 0;

    virtual LikelihoodModel& getLikelihoodModel() = 0;

    virtual MeasurementModel& getMeasurementModel() = 0;

    bool freeze_measurements();

    virtual std::pair<bool, Eigen::VectorXd> getLikelihood() = 0;

protected:
    virtual void correctStep(const ParticleSet& pred_particles, ParticleSet& cor_particles) = 0;

    PFCorrection() noexcept;

private:
    bool skip_ = false;
};

#endif /* PFCORRECTION_H */
