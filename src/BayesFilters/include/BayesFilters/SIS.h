/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef SIS_H
#define SIS_H

#include <BayesFilters/ParticleFilter.h>
#include <BayesFilters/ParticleSet.h>
#include <BayesFilters/PFCorrection.h>
#include <BayesFilters/PFPrediction.h>
#include <BayesFilters/Resampling.h>

#include <fstream>
#include <memory>

namespace bfl {
    class SIS;
}


class bfl::SIS : public ParticleFilter
{
public:
    SIS(unsigned int num_particle, std::size_t state_size_linear, std::size_t state_size_circular, std::unique_ptr<ParticleSetInitialization> initialization, std::unique_ptr<PFPrediction> prediction, std::unique_ptr<PFCorrection> correction, std::unique_ptr<Resampling> resampling) noexcept;

    SIS(unsigned int num_particle, std::size_t state_size_linear, std::unique_ptr<ParticleSetInitialization> initialization, std::unique_ptr<PFPrediction> prediction, std::unique_ptr<PFCorrection> correction, std::unique_ptr<Resampling> resampling) noexcept;

    SIS(const SIS& filter) noexcept = delete;

    SIS& operator=(const SIS& filter) noexcept = delete;

    SIS(SIS&& filter) noexcept = delete;

    SIS& operator=(SIS&& filter) noexcept = delete;

    virtual ~SIS() noexcept = default;


protected:
    bool initialization_step() override;

    void filtering_step() override;

    bool run_condition() override;

    std::vector<std::string> log_file_names(const std::string& folder_path, const std::string& file_name_prefix) override;

    void log() override;

    unsigned int num_particle_;

    std::size_t state_size_;

    ParticleSet pred_particle_;

    ParticleSet cor_particle_;
};

#endif /* SIS_H */
