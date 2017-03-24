#ifndef PARTICLEFILTERPREDICTION_H
#define PARTICLEFILTERPREDICTION_H

#include <memory>
#include <random>

#include "Prediction.h"
#include "StateModel.h"


namespace bfl
{

class ParticleFilterPrediction: public Prediction
{
public:
    /* Default constructor, disabled */
    ParticleFilterPrediction() = delete;

    /* PF prediction constructor */
    ParticleFilterPrediction(std::unique_ptr<StateModel> transition_model) noexcept;

    /* Destructor */
    ~ParticleFilterPrediction() noexcept override;

    /* Move constructor */
    ParticleFilterPrediction(ParticleFilterPrediction&& pf_prediction) noexcept;

    /* Move assignment operator */
    ParticleFilterPrediction& operator=(ParticleFilterPrediction&& pf_prediction) noexcept;

    void predict(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> pred_state) override;

protected:
    std::unique_ptr<StateModel> transition_model_;
};

} // namespace bfl

#endif /* PARTICLEFILTERPREDICTION_H */
