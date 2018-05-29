#ifndef UPDATEPARTICLES_H
#define UPDATEPARTICLES_H

#include <BayesFilters/PFCorrection.h>

#include <memory>
#include <random>

namespace bfl {
    class UpdateParticles;
}


class bfl::UpdateParticles : public PFCorrection
{
public:
    UpdateParticles() noexcept;

    virtual ~UpdateParticles() noexcept;

    std::pair<bool, Eigen::VectorXf> getLikelihood() override;

protected:
    void correctStep(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights,
                     Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) override;

    std::pair<bool, Eigen::VectorXf> likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations) override;

    bool valid_likelihood_ = false;
    Eigen::VectorXf likelihood_;
};

#endif /* UPDATEPARTICLES_H */
