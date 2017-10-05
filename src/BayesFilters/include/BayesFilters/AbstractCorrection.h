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

    virtual void correct(Eigen::Ref<Eigen::VectorXf> states, Eigen::Ref<Eigen::VectorXf> weights, const Eigen::Ref<const Eigen::MatrixXf>& measurements) = 0;

    virtual void innovation(const Eigen::Ref<const Eigen::VectorXf>& states, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::MatrixXf> innovations) = 0;

    virtual void likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations, Eigen::Ref<Eigen::VectorXf> weights) = 0;

    virtual bool setModelProperty(const std::string& property) = 0;

    virtual void observeState(const Eigen::Ref<const Eigen::MatrixXf>& states, Eigen::Ref<Eigen::MatrixXf> observations) = 0;

    virtual void measureState(const Eigen::Ref<const Eigen::MatrixXf>& states, Eigen::Ref<Eigen::MatrixXf> measurements) = 0;
};

#endif /* ABSTRACTCORRECTION_H */
