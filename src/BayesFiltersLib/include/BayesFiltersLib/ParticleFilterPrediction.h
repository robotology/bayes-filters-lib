#ifndef PREDICTION_H
#define PREDICTION_H

#include <Eigen/Dense>
#include <string>

namespace bfl
{
    class ParticleFilterPrediction;
}


class bfl::ParticleFilterPrediction
{
public:
    virtual ~ParticleFilterPrediction() noexcept { };

    virtual void predict(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> pred_state) = 0;

    virtual void motion(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> prop_state) = 0;

    virtual void motionDisturbance(Eigen::Ref<Eigen::VectorXf> sample) = 0;

    virtual bool setMotionModelProperty(const std::string& property) = 0;
};

#endif /* PREDICTION_H */
