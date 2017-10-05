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
    { };

    virtual ~VisualCorrection() noexcept { };

    VisualCorrection(VisualCorrection&& vpf_correction) noexcept :
        obs_model_(std::move(vpf_correction.visual_obs_model_))
    { };

    VisualCorrection& operator=(VisualCorrection&& vpf_correction) noexcept
    {
        visual_obs_model_ = std::move(vpf_correction.visual_obs_model_);

        return *this;
    }

    void observe(const Eigen::Ref<const Eigen::MatrixXf>& cur_state, cv::OutputArray observation) override
    {
        visual_obs_model_->observe(cur_state, observation);
    };

protected:
    std::unique_ptr<VisualObservationModel> visual_obs_model_;
};

#endif /* VISUALCORRECTION_H */
