#ifndef DRAWPARTICLES_H
#define DRAWPARTICLES_H

#include "PFPrediction.h"

#include <random>
#include <memory>

namespace bfl {
    class DrawParticles;
}


class bfl::DrawParticles: public PFPrediction
{
public:
    DrawParticles() noexcept;

    DrawParticles(DrawParticles&& draw_particles) noexcept;

    ~DrawParticles() noexcept;

    virtual StateModel& getStateModel() override;

    virtual void setStateModel(std::unique_ptr<StateModel> state_model) override;

protected:
    void predictStep(const Eigen::Ref<const Eigen::MatrixXf>& prev_states, const Eigen::Ref<const Eigen::VectorXf>& prev_weights,
                     Eigen::Ref<Eigen::MatrixXf> pred_states, Eigen::Ref<Eigen::VectorXf> pred_weights) override;

    std::unique_ptr<StateModel> state_model_;
};

#endif /* DRAWPARTICLES_H */
