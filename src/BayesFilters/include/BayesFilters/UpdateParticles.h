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

    void setMeasurementModel(std::unique_ptr<MeasurementModel> measurement_model) override;

    void setLikelihoodModel(std::unique_ptr<LikelihoodModel> likelihood) override;

    std::pair<bool, Eigen::VectorXf> getLikelihood() override;

protected:
    std::unique_ptr<MeasurementModel> measurement_model_;

    std::unique_ptr<LikelihoodModel> likelihood_model_;

    MeasurementModel& getMeasurementModel() override;

    LikelihoodModel& getLikelihoodModel() override;

    void correctStep(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights,
                     Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) override;

    bool valid_likelihood_ = false;
    Eigen::VectorXf likelihood_;
};

#endif /* UPDATEPARTICLES_H */
