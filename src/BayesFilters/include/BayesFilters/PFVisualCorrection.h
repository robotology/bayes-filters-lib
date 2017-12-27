#ifndef PFVISUALCORRECTION_H
#define PFVISUALCORRECTION_H

#include "VisualObservationModel.h"

#include <memory>

#include <Eigen/Dense>
#include <opencv2/core/core.hpp>

namespace bfl {
    class PFVisualCorrection;
}


class bfl::PFVisualCorrection
{
public:
    virtual ~PFVisualCorrection() noexcept;

    void correct(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights, cv::InputArray measurements,
                 Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights);

    virtual void innovation(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> innovations) = 0;

    virtual double likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations) = 0;


    bool skip(const bool status);


    virtual VisualObservationModel& getVisualObservationModel() = 0;

    virtual void setVisualObservationModel(std::unique_ptr<VisualObservationModel> visual_observation_model) = 0;

protected:
    PFVisualCorrection() noexcept;

    PFVisualCorrection(PFVisualCorrection&& pf_correction) noexcept;

    virtual void correctStep(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights, cv::InputArray measurements,
                             Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) = 0;

private:
    bool skip_ = false;

    friend class PFVisualCorrectionDecorator;
};

#endif /* PFVISUALCORRECTION_H */
