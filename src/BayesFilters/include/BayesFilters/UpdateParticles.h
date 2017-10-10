#ifndef UPDATEPARTICLES_H
#define UPDATEPARTICLES_H

#include "PFCorrection.h"

#include <memory>
#include <random>

namespace bfl {
    class UpdateParticles;
}


class bfl::UpdateParticles : public PFCorrection
{
public:
    UpdateParticles(std::unique_ptr<ObservationModel> obs_model) noexcept;

    UpdateParticles(UpdateParticles&& pf_correction) noexcept;

    ~UpdateParticles() noexcept override;

    UpdateParticles& operator=(UpdateParticles&& pf_correction) noexcept;

    void correct(Eigen::Ref<Eigen::VectorXf> states, Eigen::Ref<Eigen::VectorXf> weights, const Eigen::Ref<const Eigen::MatrixXf>& measurements) override;

    void innovation(const Eigen::Ref<const Eigen::VectorXf>& states, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::MatrixXf> innovation) override;

    void likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations, Eigen::Ref<Eigen::VectorXf> weights) override;

    ObservationModel getObservationModel() override;

protected:
    std::unique_ptr<ObservationModel> obs_model_;
};

#endif /* UPDATEPARTICLES_H */
