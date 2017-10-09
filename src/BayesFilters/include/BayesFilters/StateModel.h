#ifndef STATEMODEL_H
#define STATEMODEL_H

#include <Eigen/Dense>

namespace bfl {
    class StateModel;
}


class bfl::StateModel
{
public:
    virtual ~StateModel() noexcept { };

    virtual void propagate(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> prop_state) = 0;

    virtual void motion(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> mot_state) = 0;

    virtual Eigen::VectorXf getNoiseSample() = 0;

    virtual Eigen::MatrixXf getNoiseCovariance() = 0;

    virtual bool setProperty(const std::string& property) = 0;
};

#endif /* STATEMODEL_H */
