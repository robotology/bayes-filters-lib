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
    DrawParticles() noexcept;

    DrawParticles(DrawParticles&& pf_prediction) noexcept;

    ~DrawParticles() noexcept;

    DrawParticles& operator=(DrawParticles&& pf_prediction) noexcept;

protected:
    void predictStep(const Eigen::Ref<const Eigen::MatrixXf>& prev_states, const Eigen::Ref<const Eigen::VectorXf>& prev_weights,
                     Eigen::Ref<Eigen::MatrixXf> pred_states, Eigen::Ref<Eigen::VectorXf> pred_weights) override;
};

#endif /* DRAWPARTICLES_H */
