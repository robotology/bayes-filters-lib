#ifndef PARTICLEFILTERCORRECTION_H
#define PARTICLEFILTERCORRECTION_H

#include <memory>
#include <random>

#include "Correction.h"
#include "ObservationModel.h"

namespace bfl
{
    class ParticleFilterCorrection;
}


class bfl::ParticleFilterCorrection: public Correction
{
public:
    /* Default constructor, disabled */
    ParticleFilterCorrection() = delete;

    /* PF correction constructor */
    ParticleFilterCorrection(std::unique_ptr<ObservationModel> observation_model) noexcept;

    /* Destructor */
    ~ParticleFilterCorrection() noexcept override;

    /* Move constructor */
    ParticleFilterCorrection(ParticleFilterCorrection&& pf_correction) noexcept;

    /* Move assignment operator */
    ParticleFilterCorrection& operator=(ParticleFilterCorrection&& pf_correction) noexcept;

    void correct(const Eigen::Ref<const Eigen::VectorXf>& pred_state, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::VectorXf> cor_state) override;

    void virtual_observation(const Eigen::Ref<const Eigen::VectorXf>& state, Eigen::Ref<Eigen::MatrixXf> virtual_measurements) override;

    void innovation(const Eigen::Ref<const Eigen::VectorXf>& pred_state, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::MatrixXf> innovation) override;

    void likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovation, Eigen::Ref<Eigen::VectorXf> cor_state) override;

    /* TEMPORARY - WILL BE REMOVED AFTER IMPLEMENTING Initialization CLASS */
    void observation(const Eigen::Ref<const Eigen::VectorXf>& state, Eigen::Ref<Eigen::MatrixXf> measurements);

protected:
    std::unique_ptr<ObservationModel> measurement_model_;
};

#endif /* PARTICLEFILTERCORRECTION_H */
