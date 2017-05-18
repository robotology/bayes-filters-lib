#ifndef DRAWPARTICLE_H
#define DRAWPARTICLE_H

#include <random>
#include <memory>

#include "ParticleFilterPrediction.h"
#include "StateModel.h"

namespace bfl {
    class DrawParticle;
}


class bfl::DrawParticle: public ParticleFilterPrediction
{
public:
    /* Default constructor, disabled */
    DrawParticle() = delete;

    /* PF prediction constructor */
    DrawParticle(std::unique_ptr<StateModel> transition_model) noexcept;

    /* Destructor */
    ~DrawParticle() noexcept override;

    /* Move constructor */
    DrawParticle(DrawParticle&& pf_prediction) noexcept;

    /* Move assignment operator */
    DrawParticle& operator=(DrawParticle&& pf_prediction) noexcept;

    void predict(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> pred_state) override;

    void motion(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> prop_state) override;

    void motionDisturbance(Eigen::Ref<Eigen::VectorXf> sample) override;

    bool setStateModelProperty(const std::string& property) override;

protected:
    std::unique_ptr<StateModel> state_model_;
};

#endif /* DRAWPARTICLE_H */
