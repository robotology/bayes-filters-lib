#ifndef VISUALCORRECTION_H
#define VISUALCORRECTION_H

#include "AbstractVisualCorrection.h"
#include "VisualObservationModel.h"

namespace bfl {
    class VisualCorrection;
}


class bfl::VisualCorrection : public AbstractVisualCorrection
{
public:
    VisualCorrection(std::unique_ptr<VisualObservationModel> visual_obs_model) noexcept :
        visual_obs_model_(std::move(visual_obs_model))
    { }

    virtual ~VisualCorrection() noexcept { };

    VisualCorrection(VisualCorrection&& vpf_correction) noexcept :
        obs_model_(std::move(vpf_correction.visual_obs_model_))
    { }

    VisualCorrection& operator=(VisualCorrection&& vpf_correction) noexcept
    {
        visual_obs_model_ = std::move(vpf_correction.visual_obs_model_);

        return *this;
    }

    void observeState(const Eigen::Ref<const Eigen::MatrixXf>& states, cv::OutputArray observations) override
    {
        visual_obs_model_->observe(states, observation);
    }

    void measureState(const Eigen::Ref<const Eigen::MatrixXf>& states, cv::OutputArray measurements) override
    {
        visual_obs_model_->measure(states, measurements);
    }

protected:
    std::unique_ptr<VisualObservationModel> visual_obs_model_;
};

#endif /* VISUALCORRECTION_H */
