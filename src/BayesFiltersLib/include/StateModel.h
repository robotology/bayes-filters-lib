#ifndef STATEMODEL_H
#define STATEMODEL_H

#include <Eigen/Dense>


class StateModel
{
public:
    StateModel() = default;

    virtual ~StateModel() noexcept { };

    virtual void propagate(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> prop_state) = 0;

    virtual void noiseSample(Eigen::Ref<Eigen::VectorXf> sample) = 0;

    virtual void motion(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> next_state) = 0;
};

#endif /* STATEMODEL_H */
