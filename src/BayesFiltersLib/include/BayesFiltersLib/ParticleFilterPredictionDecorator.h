#ifndef PREDICTIONDECORATOR_H
#define PREDICTIONDECORATOR_H

#include <memory>

#include "ParticleFilterPrediction.h"

namespace bfl
{
    class ParticleFilterPredictionDecorator;
}


class bfl::ParticleFilterPredictionDecorator : public ParticleFilterPrediction {
public:
    void predict(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> pred_state) override;

    void motion(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> prop_state) override;

    void motionDisturbance(Eigen::Ref<Eigen::VectorXf> sample) override;

    bool setMotionModelProperty(const std::string& property) override;

protected:
    /* Default constructor, disabled */
    ParticleFilterPredictionDecorator() = delete;

    /* Decorator constructor */
    ParticleFilterPredictionDecorator(std::unique_ptr<ParticleFilterPrediction> prediction) noexcept;

    /* Destructor */
    ~ParticleFilterPredictionDecorator() noexcept override;

    /* Move constructor */
    ParticleFilterPredictionDecorator(ParticleFilterPredictionDecorator&& prediction) noexcept;

    /* Move assignment operator */
    ParticleFilterPredictionDecorator& operator=(ParticleFilterPredictionDecorator&& prediction) noexcept;

private:
    std::unique_ptr<ParticleFilterPrediction> prediction_;
};

#endif /* PREDICTIONDECORATOR_H */
