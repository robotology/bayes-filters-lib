#ifndef PREDICTIONDECORATOR_H
#define PREDICTIONDECORATOR_H

#include <memory>

#include "ParticleFilterPrediction.h"

namespace bfl
{
    class PredictionDecorator;
}


class bfl::PredictionDecorator : public ParticleFilterPrediction {
public:
    void predict(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> pred_state) override;

    void motion(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> prop_state) override;

    void motionDisturbance(Eigen::Ref<Eigen::VectorXf> sample) override;

    bool setMotionModelProperty(const std::string& property) override;

protected:
    /* Default constructor, disabled */
    PredictionDecorator() = delete;

    /* Decorator constructor */
    PredictionDecorator(std::unique_ptr<ParticleFilterPrediction> prediction) noexcept;

    /* Destructor */
    ~PredictionDecorator() noexcept override;

    /* Move constructor */
    PredictionDecorator(PredictionDecorator&& prediction) noexcept;

    /* Move assignment operator */
    PredictionDecorator& operator=(PredictionDecorator&& prediction) noexcept;

private:
    std::unique_ptr<ParticleFilterPrediction> prediction_;
};

#endif /* PREDICTIONDECORATOR_H */
