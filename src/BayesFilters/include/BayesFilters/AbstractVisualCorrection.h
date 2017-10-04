#ifndef ABSTRACTVISUALCORRECTION_H
#define ABSTRACTVISUALCORRECTION_H

#include "VisualObservationModel.h"

#include <Eigen/Dense>
#include <opencv2/core/core.hpp>

namespace bfl {
    class AbstractVisualCorrection;
}


class bfl::AbstractVisualCorrection
{
public:
    virtual ~AbstractVisualCorrection() noexcept { };

    virtual void correct(const Eigen::Ref<const Eigen::MatrixXf>& pred_state, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> cor_state) = 0;

    virtual void innovation(const Eigen::Ref<const Eigen::MatrixXf>& pred_state, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> innovation) = 0;

    virtual void likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovation, Eigen::Ref<Eigen::MatrixXf> cor_state) = 0;

    virtual bool setObservationModelProperty(const std::string& property) = 0;

    virtual void observe(const Eigen::Ref<const Eigen::MatrixXf>& cur_state, cv::OutputArray observation) = 0;
};

#endif /* ABSTRACTVISUALCORRECTION_H */
