#ifndef ABSTRACTCORRECTION_H
#define ABSTRACTCORRECTION_H

#include <Eigen/Dense>

namespace bfl {
    class AbstractCorrection;
}


class bfl::AbstractCorrection
{
public:
    virtual ~AbstractCorrection() noexcept { };

    virtual void correct(const Eigen::Ref<const Eigen::VectorXf>& pred_state, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::VectorXf> cor_state) = 0;

    virtual void virtual_observation(const Eigen::Ref<const Eigen::VectorXf>& state, Eigen::Ref<Eigen::MatrixXf> virtual_measurements) = 0;

    virtual void innovation(const Eigen::Ref<const Eigen::VectorXf>& pred_state, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::MatrixXf> innovation) = 0;

    virtual void likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovation, Eigen::Ref<Eigen::VectorXf> cor_state) = 0;
};

#endif /* ABSTRACTCORRECTION_H */
