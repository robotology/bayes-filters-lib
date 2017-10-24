#ifndef SISPARTICLEFILTER_H
#define SISPARTICLEFILTER_H

#include "FilteringAlgorithm.h"
#include "PFCorrection.h"
#include "PFPrediction.h"
#include "Resampling.h"

#include <memory>

#include <Eigen/Dense>

namespace bfl {
    class SISParticleFilter;
}


class bfl::SISParticleFilter : public FilteringAlgorithm
{
public:
    SISParticleFilter() = delete;

    SISParticleFilter(std::unique_ptr<PFPrediction> prediction, std::unique_ptr<PFCorrection> correction, std::unique_ptr<Resampling> resampling) noexcept;

    SISParticleFilter(SISParticleFilter&& sir_pf) noexcept;

    ~SISParticleFilter() noexcept;

    SISParticleFilter& operator=(SISParticleFilter&& sir_pf) noexcept;

    void initialization() override;

    void filteringStep() override;

    void getResult() override;

    bool runCondition() override { return (getFilteringStep() < simulation_time_); };

protected:
    std::unique_ptr<PFPrediction> prediction_;
    std::unique_ptr<PFCorrection> correction_;
    std::unique_ptr<Resampling>   resampling_;

    int                           simulation_time_;
    int                           num_particle_;
    int                           surv_x_;
    int                           surv_y_;

    Eigen::MatrixXf               object_;
    Eigen::MatrixXf               measurement_;

    Eigen::MatrixXf               pred_particle_;
    Eigen::VectorXf               pred_weight_;

    Eigen::MatrixXf               cor_particle_;
    Eigen::VectorXf               cor_weight_;

    std::vector<Eigen::MatrixXf>  result_pred_particle_;
    std::vector<Eigen::VectorXf>  result_pred_weight_;

    std::vector<Eigen::MatrixXf>  result_cor_particle_;
    std::vector<Eigen::VectorXf>  result_cor_weight_;

    void snapshot();
};

#endif /* SISPARTICLEFILTER_H */
