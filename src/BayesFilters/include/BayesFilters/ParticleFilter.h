/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef PARTICLEFILTER_H
#define PARTICLEFILTER_H

#include <BayesFilters/FilteringAlgorithm.h>
#include <BayesFilters/ParticleSetInitialization.h>
#include <BayesFilters/PFCorrection.h>
#include <BayesFilters/PFPrediction.h>
#include <BayesFilters/Resampling.h>

#include <memory>

namespace bfl{
    class ParticleFilter;
}


class bfl::ParticleFilter : public FilteringAlgorithm
{
public:
    virtual bool skip(const std::string& what_step, const bool status) override;

    virtual ~ParticleFilter() noexcept = default;


protected:
    ParticleFilter(std::unique_ptr<ParticleSetInitialization> initialization, std::unique_ptr<PFPrediction> prediction, std::unique_ptr<PFCorrection> correction, std::unique_ptr<Resampling> resampling) noexcept;

    ParticleFilter(ParticleFilter&& pf) noexcept;

    ParticleFilter& operator=(ParticleFilter&& pf) noexcept;

    std::unique_ptr<ParticleSetInitialization> initialization_;

    std::unique_ptr<PFPrediction> prediction_;

    std::unique_ptr<PFCorrection> correction_;

    std::unique_ptr<Resampling> resampling_;
};

#endif /* PARTICLEFILTER_H */
