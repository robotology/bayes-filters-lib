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

    void enableLog(const std::string& prefix_name) override;

    void disableLog() override;

protected:
    unsigned int num_particle_;

    Eigen::MatrixXf pred_particle_;

    Eigen::VectorXf pred_weight_;

    Eigen::MatrixXf cor_particle_;

    Eigen::VectorXf cor_weight_;

private:
    bool log_enabled_ = false;

    std::string prefix_name_;

    mutable std::ofstream log_file_pred_particle_;

    mutable std::ofstream log_file_pred_weight_;

    mutable std::ofstream log_file_cor_particle_;

    mutable std::ofstream log_file_cor_weight_;

    void logger(const Eigen::Ref<const Eigen::MatrixXf>& pred_particle, const Eigen::Ref<const Eigen::VectorXf>& pred_weight,
                const Eigen::Ref<const Eigen::MatrixXf>& cor_particle,  const Eigen::Ref<const Eigen::VectorXf>& cor_weight) const;
};

#endif /* SIS_H */
