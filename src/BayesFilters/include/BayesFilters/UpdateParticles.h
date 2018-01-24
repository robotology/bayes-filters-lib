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
    UpdateParticles() noexcept;

    UpdateParticles(UpdateParticles&& pf_correction) noexcept;

    virtual ~UpdateParticles() noexcept;


    void innovation(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::MatrixXf> innovations) override;

    double likelihood(const Eigen::Ref<const Eigen::VectorXf>& innovation) override;

    virtual ObservationModel& getObservationModel() override;

    virtual void setObservationModel(std::unique_ptr<ObservationModel> observation_model) override;

protected:
    void correctStep(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights, const Eigen::Ref<const Eigen::MatrixXf>& measurements,
                     Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) override;

    std::unique_ptr<ObservationModel> observation_model_;
};

#endif /* UPDATEPARTICLES_H */
