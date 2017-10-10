#ifndef DRAWPARTICLE_H
#define DRAWPARTICLE_H

#include <random>
#include <memory>

#include "PFPrediction.h"
#include "StateModel.h"

namespace bfl {
    class DrawParticle;
}


class bfl::DrawParticle: public PFPrediction
{
public:
    DrawParticle(std::unique_ptr<StateModel> state_model) noexcept;

    DrawParticle(DrawParticle&& pf_prediction) noexcept;

    ~DrawParticle() noexcept override;

    DrawParticle& operator=(DrawParticle&& pf_prediction) noexcept;

    void predict(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> pred_state) override;

    StateModel getStateModel() override;

protected:
    std::unique_ptr<StateModel> state_model_;
};

#endif /* DRAWPARTICLE_H */
