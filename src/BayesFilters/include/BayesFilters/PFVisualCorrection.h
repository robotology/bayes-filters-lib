#ifndef PFVISUALCORRECTION_H
#define PFVISUALCORRECTION_H

#include "VisualObservationModel.h"

#include <Eigen/Dense>
#include <opencv2/core/core.hpp>

namespace bfl {
    class PFVisualCorrection;
}


class bfl::PFVisualCorrection
{
public:
    virtual ~PFVisualCorrection() noexcept { };

    virtual void correct(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights, cv::InputArray measurements,
                         Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) = 0;

    virtual void innovation(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> innovations) = 0;

    virtual double likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations) = 0;

    virtual VisualObservationModel& getVisualObservationModel() = 0;
};

#endif /* PFVISUALCORRECTION_H */
