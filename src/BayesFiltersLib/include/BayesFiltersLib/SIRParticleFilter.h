#ifndef SIRPARTICLEFILTER_H
#define SIRPARTICLEFILTER_H

#include <memory>
#include <random>

#include <Eigen/Dense>

#include "FilteringAlgorithm.h"
#include "StateModel.h"
#include "Prediction.h"
#include "ObservationModel.h"
#include "Correction.h"
#include "Resampling.h"


namespace bfl
{

class SIRParticleFilter: public FilteringAlgorithm {
public:
    
    /* Default constructor, disabled */
    SIRParticleFilter() = delete;

    /* SIR complete constructor */
    SIRParticleFilter(std::unique_ptr<Prediction> prediction, std::unique_ptr<Correction> correction, std::unique_ptr<Resampling> resampling) noexcept;

    /* Destructor */
    ~SIRParticleFilter() noexcept override;

    /* Move constructor */
    SIRParticleFilter(SIRParticleFilter&& sir_pf) noexcept;

    /* Move assignment operator */
    SIRParticleFilter& operator=(SIRParticleFilter&& sir_pf) noexcept;

    void runFilter() override;

    void getResult() override;

protected:
    std::unique_ptr<Prediction>       prediction_;
    std::unique_ptr<Correction>       correction_;
    std::unique_ptr<Resampling>       resampling_;

    Eigen::MatrixXf                   object_;
    Eigen::MatrixXf                   measurement_;

    Eigen::MatrixXf                   init_particle_;
    Eigen::VectorXf                   init_weight_;

    std::vector<Eigen::MatrixXf>      result_particle_;
    std::vector<Eigen::VectorXf>      result_weight_;

    void snapshot();
};

} // namespace bfl

#endif /* SIRPARTICLEFILTER_H */
