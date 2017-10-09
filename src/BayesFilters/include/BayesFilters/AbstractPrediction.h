#ifndef ABSTRACTPREDICTION_H
#define ABSTRACTPREDICTION_H

#include <Eigen/Dense>
#include <string>

namespace bfl {
    class AbstractPrediction;
}


class bfl::AbstractPrediction
{
public:
    virtual ~AbstractPrediction() noexcept { };

    virtual void predict(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> pred_state) = 0;

    virtual void motion(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> prop_state) = 0;

    virtual void motionDisturbance(Eigen::Ref<Eigen::VectorXf> sample) = 0;

    virtual bool setModelProperty(const std::string& property) = 0;
};

#endif /* ABSTRACTPREDICTION_H */
