#ifndef ADDITIVESTATEMODEL_H
#define ADDITIVESTATEMODEL_H

#include <Eigen/Dense>

#include <BayesFilters/StateModel.h>

namespace bfl {
    class AdditiveStateModel;
}


class bfl::AdditiveStateModel : public bfl::StateModel
{
public:
    virtual ~AdditiveStateModel() noexcept { };

    virtual void motion(const Eigen::Ref<const Eigen::MatrixXf>& cur_states,
                        Eigen::Ref<Eigen::MatrixXf> mot_states) override;

    virtual Eigen::MatrixXf getNoiseCovarianceMatrix() = 0;

    virtual Eigen::MatrixXf getNoiseSample(const std::size_t num) = 0;
};

#endif /* ADDITIVESTATEMODEL_H */
