#ifndef SIS_H
#define SIS_H

#include <BayesFilters/ParticleFilter.h>
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
    SIS(unsigned int num_particle) noexcept;

    SIS(SIS&& sir_pf) noexcept;

    virtual ~SIS() noexcept;

    SIS& operator=(SIS&& sir_pf) noexcept;

    bool initialization() override;

    void filteringStep() override;

    bool runCondition() override;

protected:
    unsigned int num_particle_;

    Eigen::MatrixXf pred_particle_;

    Eigen::VectorXf pred_weight_;

    Eigen::MatrixXf cor_particle_;

    Eigen::VectorXf cor_weight_;

    std::vector<std::string> log_filenames(const std::string& prefix_path, const std::string& prefix_name) override
    {
        return {prefix_path + "/" + prefix_name + "_pred_particles",
                prefix_path + "/" + prefix_name + "_pred_weights",
                prefix_path + "/" + prefix_name + "_cor_particles",
                prefix_path + "/" + prefix_name + "_cor_weights"};
    }
};

#endif /* SIS_H */
