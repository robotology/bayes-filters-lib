#ifndef STATEMODEL_H
#define STATEMODEL_H

#include <Eigen/Dense>

namespace bfl
{
    class StateModel;
}


class bfl::StateModel
{
public:
    virtual ~StateModel() noexcept { };

    virtual void propagate(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> prop_state) = 0;

    virtual void noiseSample(Eigen::Ref<Eigen::VectorXf> sample) = 0;

    virtual bool setProperty(const std::string& property) = 0;
};

#endif /* STATEMODEL_H */
