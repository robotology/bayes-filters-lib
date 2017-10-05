#ifndef PARTICLEFILTERCORRECTION_H
#define PARTICLEFILTERCORRECTION_H

#include <memory>
#include <random>

#include "Correction.h"

namespace bfl {
    class ParticleFilterCorrection;
}


class bfl::ParticleFilterCorrection: public Correction
{
public:
    ParticleFilterCorrection(std::unique_ptr<ObservationModel> observation_model) noexcept;

    ~ParticleFilterCorrection() noexcept override;

    ParticleFilterCorrection(ParticleFilterCorrection&& pf_correction) noexcept;

    ParticleFilterCorrection& operator=(ParticleFilterCorrection&& pf_correction) noexcept;

    void correct(Eigen::Ref<Eigen::VectorXf> states, Eigen::Ref<Eigen::VectorXf> weights, const Eigen::Ref<const Eigen::MatrixXf>& measurements) override;

    void innovation(const Eigen::Ref<const Eigen::VectorXf>& states, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::MatrixXf> innovation) override;

    void likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations, Eigen::Ref<Eigen::VectorXf> weights) override;

    bool setObservationModelProperty(const std::string& property) override;
};

#endif /* PARTICLEFILTERCORRECTION_H */
