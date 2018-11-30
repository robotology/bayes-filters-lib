#ifndef LTISTATEMODEL_H
#define LTISTATEMODEL_H

#include <Eigen/Dense>

#include <BayesFilters/LinearStateModel.h>

namespace bfl {
    class LTIStateModel;
}


class bfl::LTIStateModel : public bfl::LinearStateModel
{
public:
    LTIStateModel(Eigen::MatrixXf& transition_matrix,
                  Eigen::MatrixXf& noise_covariance_matrix);

    virtual ~LTIStateModel() noexcept { };

    void propagate(const Eigen::Ref<const Eigen::MatrixXf>& cur_states,
                   Eigen::Ref<Eigen::MatrixXf> prop_states) override;

    Eigen::MatrixXf getNoiseCovarianceMatrix() override;

    Eigen::MatrixXf getStateTransitionMatrix() override;

    bool setProperty(const std::string& property) override;

    Eigen::MatrixXf getJacobian() override;

protected:
    /*
     * State transition matrix.
     */
    Eigen::MatrixXf F_;

    /*
     * Noise covariance matrix of zero mean additive white noise.
     */
    Eigen::MatrixXf Q_;
};

#endif /* LTISTATEMODEL_H */
