#ifndef SIRPARTICLEFILTER_H
#define SIRPARTICLEFILTER_H

#include <memory>
#include <random>

#include <Eigen/Dense>

#include "FilteringAlgorithm.h"
#include "StateModel.h"
#include "PFPrediction.h"
#include "ObservationModel.h"
#include "Correction.h"
#include "Resampling.h"

namespace bfl {
    class SIRParticleFilter;
}


class bfl::SIRParticleFilter : public FilteringAlgorithm
{
public:
    /* Default constructor, disabled */
    SIRParticleFilter() = delete;

    /* SIR complete constructor */
    SIRParticleFilter(std::unique_ptr<PFPrediction> prediction, std::unique_ptr<Correction> correction, std::unique_ptr<Resampling> resampling) noexcept;

    /* Destructor */
    ~SIRParticleFilter() noexcept override;

    /* Move constructor */
    SIRParticleFilter(SIRParticleFilter&& sir_pf) noexcept;

    /* Move assignment operator */
    SIRParticleFilter& operator=(SIRParticleFilter&& sir_pf) noexcept;

    void initialization() override;

    void filteringStep() override;

    void getResult() override;

    bool runCondition() override { return (getFilteringStep() < simulation_time_); };

protected:
    std::unique_ptr<PFPrediction> prediction_;
    std::unique_ptr<Correction>   correction_;
    std::unique_ptr<Resampling>   resampling_;

    int                           simulation_time_;
    int                           num_particle_;
    int                           surv_x_;
    int                           surv_y_;

    Eigen::MatrixXf               object_;
    Eigen::MatrixXf               measurement_;

    Eigen::MatrixXf               init_particle_;
    Eigen::VectorXf               init_weight_;

    std::vector<Eigen::MatrixXf>  result_particle_;
    std::vector<Eigen::VectorXf>  result_weight_;

    void snapshot();
};

#endif /* SIRPARTICLEFILTER_H */
