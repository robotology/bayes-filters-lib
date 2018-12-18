#ifndef SIS_H
#define SIS_H

#include <BayesFilters/ParticleFilter.h>
#include <BayesFilters/ParticleSet.h>
#include <BayesFilters/PFCorrection.h>
#include <BayesFilters/PFPrediction.h>
#include <BayesFilters/Resampling.h>

#include <fstream>
#include <memory>

#include <Eigen/Dense>

namespace bfl {
    class SIS;
}


class bfl::SIS : public ParticleFilter
{
public:
    SIS(unsigned int num_particle, std::size_t state_size_linear, std::unique_ptr<ParticleSetInitialization> initialization, std::unique_ptr<PFPrediction> prediction, std::unique_ptr<PFCorrection> correction, std::unique_ptr<Resampling> resampling) noexcept;

    SIS(unsigned int num_particle, std::size_t state_size_linear, std::size_t state_size_circular, std::unique_ptr<ParticleSetInitialization> initialization, std::unique_ptr<PFPrediction> prediction, std::unique_ptr<PFCorrection> correction, std::unique_ptr<Resampling> resampling) noexcept;

    SIS(SIS&& sir_pf) noexcept;

    virtual ~SIS() noexcept;

    SIS& operator=(SIS&& sir_pf) noexcept;

    bool initialization() override;

    bool runCondition() override;

protected:
    unsigned int num_particle_;

    std::size_t state_size_;

    ParticleSet pred_particle_;

    ParticleSet cor_particle_;

    void filteringStep() override;

    std::vector<std::string> log_filenames(const std::string& prefix_path, const std::string& prefix_name) override
    {
        return {prefix_path + "/" + prefix_name + "_pred_particles",
                prefix_path + "/" + prefix_name + "_pred_weights",
                prefix_path + "/" + prefix_name + "_cor_particles",
                prefix_path + "/" + prefix_name + "_cor_weights"};
    }

    void log() override;
};

#endif /* SIS_H */
