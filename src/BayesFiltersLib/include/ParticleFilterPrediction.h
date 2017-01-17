#ifndef PARTICLEFILTERPREDICTION_H
#define PARTICLEFILTERPREDICTION_H

#include <memory>
#include <random>

#include "Prediction.h"
#include "StateModel.h"


class ParticleFilterPrediction: public Prediction
{
public:
    /* Default constructor, disabled */
    ParticleFilterPrediction() = delete;

    /* PF prediction constructor */
    ParticleFilterPrediction(std::shared_ptr<StateModel> transition_model) noexcept;

    /* Destructor */
    ~ParticleFilterPrediction() noexcept override;

    /* Copy constructor */
    ParticleFilterPrediction(const ParticleFilterPrediction& pf_prediction);

    /* Move constructor */
    ParticleFilterPrediction(ParticleFilterPrediction&& pf_prediction) noexcept;

    /* Copy assignment operator */
    ParticleFilterPrediction& operator=(const ParticleFilterPrediction& pf_prediction);

    /* Move assignment operator */
    ParticleFilterPrediction& operator=(ParticleFilterPrediction&& pf_prediction) noexcept;

    void predict(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> pred_state) override;

protected:
    std::shared_ptr<StateModel> transition_model_;
};

#endif /* PARTICLEFILTERPREDICTION_H */
