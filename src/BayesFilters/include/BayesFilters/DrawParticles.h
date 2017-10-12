#ifndef DRAWPARTICLES_H
#define DRAWPARTICLES_H

#include "PFPrediction.h"
#include "StateModel.h"

#include <random>
#include <memory>

namespace bfl {
    class DrawParticles;
}


class bfl::DrawParticles: public PFPrediction
{
public:
    DrawParticles(std::unique_ptr<StateModel> state_model) noexcept;

    DrawParticles(DrawParticles&& pf_prediction) noexcept;

    ~DrawParticles() noexcept;

    DrawParticles& operator=(DrawParticles&& pf_prediction) noexcept;

    void predict(const Eigen::Ref<const Eigen::MatrixXf>& prev_state, Eigen::Ref<Eigen::MatrixXf> pred_state) override;

    StateModel& getStateModel() override;

protected:
    std::unique_ptr<StateModel> state_model_;
};

#endif /* DRAWPARTICLES_H */
