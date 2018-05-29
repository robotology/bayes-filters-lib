#ifndef SIS_H
#define SIS_H

#include <BayesFilters/ParticleFilter.h>
#include <BayesFilters/PFCorrection.h>
#include <BayesFilters/PFPrediction.h>
#include <BayesFilters/Resampling.h>

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

    void getResult() override;

protected:
    unsigned int num_particle_;

    Eigen::MatrixXf object_;

    Eigen::MatrixXf measurement_;

    Eigen::MatrixXf pred_particle_;

    Eigen::VectorXf pred_weight_;

    Eigen::MatrixXf cor_particle_;

    Eigen::VectorXf cor_weight_;

    std::vector<Eigen::MatrixXf> result_pred_particle_;

    std::vector<Eigen::VectorXf> result_pred_weight_;

    std::vector<Eigen::MatrixXf> result_cor_particle_;

    std::vector<Eigen::VectorXf> result_cor_weight_;
};

#endif /* SIS_H */
