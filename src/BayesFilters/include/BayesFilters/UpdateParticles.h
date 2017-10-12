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

    ~UpdateParticles() noexcept;

    UpdateParticles& operator=(UpdateParticles&& pf_correction) noexcept;

    void correct(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights, const Eigen::Ref<const Eigen::MatrixXf>& measurements,
                 Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) override;

    void innovation(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::MatrixXf> innovations) override;

    double likelihood(const Eigen::Ref<const Eigen::VectorXf>& innovation) override;

    ObservationModel& getObservationModel() override;

protected:
    std::unique_ptr<ObservationModel> obs_model_;
};

#endif /* UPDATEPARTICLES_H */
