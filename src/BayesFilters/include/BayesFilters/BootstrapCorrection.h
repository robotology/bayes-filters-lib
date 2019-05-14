/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef BOOTSTRAPCORRECTION_H
#define BOOTSTRAPCORRECTION_H

#include <BayesFilters/ParticleSet.h>
#include <BayesFilters/PFCorrection.h>

#include <memory>
#include <random>

namespace bfl {
    class BootstrapCorrection;
}


class bfl::BootstrapCorrection : public PFCorrection
{
public:
    BootstrapCorrection(std::unique_ptr<MeasurementModel> measurement_model, std::unique_ptr<LikelihoodModel> likelihood_model) noexcept;

    BootstrapCorrection(const BootstrapCorrection& correction) noexcept = delete;

    BootstrapCorrection& operator=(const BootstrapCorrection& correction) noexcept = delete;

    BootstrapCorrection(BootstrapCorrection&& correction) noexcept;

    BootstrapCorrection& operator=(BootstrapCorrection&& correction) noexcept;

    virtual ~BootstrapCorrection() noexcept = default;

    MeasurementModel& getMeasurementModel() noexcept override;

    LikelihoodModel& getLikelihoodModel() noexcept override;

    std::pair<bool, Eigen::VectorXd> getLikelihood() override;


protected:
    void correctStep(const ParticleSet& pred_particles, ParticleSet& cor_particles) override;


private:
    std::unique_ptr<MeasurementModel> measurement_model_;

    std::unique_ptr<LikelihoodModel> likelihood_model_;

    bool valid_likelihood_ = false;

    Eigen::VectorXd likelihood_;
};

#endif /* BOOTSTRAPCORRECTION_H */
