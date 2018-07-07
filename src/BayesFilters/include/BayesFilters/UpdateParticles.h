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

    void setLikelihoodModel(std::unique_ptr<LikelihoodModel> likelihood_model) override;

    void setMeasurementModel(std::unique_ptr<MeasurementModel> measurement_model) override;

    void setProcess(std::unique_ptr<Process> process) override;

    std::pair<bool, Eigen::VectorXf> getLikelihood() override;

protected:
    std::unique_ptr<LikelihoodModel> likelihood_model_;

    std::unique_ptr<MeasurementModel> measurement_model_;

    std::unique_ptr<Process> process_;

    LikelihoodModel& getLikelihoodModel() override;

    MeasurementModel& getMeasurementModel() override;

    Process& getProcess() override;

    void correctStep(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights,
                     Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) override;

    bool valid_likelihood_ = false;
    Eigen::VectorXf likelihood_;
};

#endif /* UPDATEPARTICLES_H */
