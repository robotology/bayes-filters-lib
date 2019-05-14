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
    virtual ~PFCorrection() noexcept = default;

    void correct(const ParticleSet& pred_particles, ParticleSet& cor_particles);

    bool skip(const bool status) noexcept;

    bool freeze_measurements();

    virtual MeasurementModel& getMeasurementModel() noexcept = 0;

    virtual LikelihoodModel& getLikelihoodModel() noexcept = 0;

    virtual std::pair<bool, Eigen::VectorXd> getLikelihood() = 0;


protected:
    PFCorrection() noexcept = default;

    PFCorrection(const PFCorrection& correction) noexcept = delete;

    PFCorrection& operator=(const PFCorrection& correction) noexcept = delete;

    PFCorrection(PFCorrection&& correction) noexcept = default;

    PFCorrection& operator=(PFCorrection&& correction) noexcept = default;

    virtual void correctStep(const ParticleSet& pred_particles, ParticleSet& cor_particles) = 0;


private:
    bool skip_ = false;
};

#endif /* PFCORRECTION_H */
