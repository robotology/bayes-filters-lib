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
    SIRParticleFilter(std::shared_ptr<StateModel> state_model, std::shared_ptr<Prediction> prediction, std::shared_ptr<ObservationModel> observation_model, std::shared_ptr<Correction> correction, std::shared_ptr<Resampling> resampling) noexcept;

    /* Destructor */
    ~SIRParticleFilter() noexcept override;

    /* Copy constructor */
    SIRParticleFilter(const SIRParticleFilter& sir_pf);

    /* Move constructor */
    SIRParticleFilter(SIRParticleFilter&& sir_pf) noexcept;

    /* Copy assignment operator */
    SIRParticleFilter& operator=(const SIRParticleFilter& sir_pf);

    /* Move assignment operator */
    SIRParticleFilter& operator=(SIRParticleFilter&& sir_pf) noexcept;

    void runFilter() override;

    void getResult() override;

protected:
    std::shared_ptr<StateModel>       state_model_;
    std::shared_ptr<Prediction>       prediction_;
    std::shared_ptr<ObservationModel> observation_model_;
    std::shared_ptr<Correction>       correction_;
    std::shared_ptr<Resampling>       resampling_;

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
