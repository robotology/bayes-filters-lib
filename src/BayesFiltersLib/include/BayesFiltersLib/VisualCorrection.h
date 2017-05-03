#ifndef VISUALCORRECTION_H
#define VISUALCORRECTION_H

#include <Eigen/Dense>
#include <opencv2/core/core.hpp>

namespace bfl {
    class VisualCorrection;
}


class bfl::VisualCorrection
{
public:
    virtual ~VisualCorrection() noexcept { };

    virtual void correct(const Eigen::Ref<const Eigen::MatrixXf>& pred_state, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> cor_state) = 0;

    virtual void innovation(const Eigen::Ref<const Eigen::MatrixXf>& pred_state, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> innovation) = 0;

    virtual void likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovation, Eigen::Ref<Eigen::MatrixXf> cor_state) = 0;
};

#endif /* VISUALCORRECTION_H */
