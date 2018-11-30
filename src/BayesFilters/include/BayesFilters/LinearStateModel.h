#ifndef LINEARSTATEMODEL_H
#define LINEARSTATEMODEL_H

#include <Eigen/Dense>

#include <BayesFilters/AdditiveStateModel.h>

namespace bfl {
    class LinearStateModel;
}


class bfl::LinearStateModel : public bfl::AdditiveStateModel
{
public:
    virtual ~LinearStateModel() noexcept { };

    virtual void propagate(const Eigen::Ref<const Eigen::MatrixXf>& cur_states, Eigen::Ref<Eigen::MatrixXf> mot_states) override;

    virtual Eigen::MatrixXf getStateTransitionMatrix() = 0;
};

#endif /* LINEARSTATEMODEL_H */
