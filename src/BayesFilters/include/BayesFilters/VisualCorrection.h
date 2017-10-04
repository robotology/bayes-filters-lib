#ifndef VISUALCORRECTION_H
#define VISUALCORRECTION_H

#include "AbstractVisualCorrection.h"
#include "VisualObservationModel.h"

#include <Eigen/Dense>
#include <opencv2/core/core.hpp>

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

    void observe(const Eigen::Ref<const Eigen::MatrixXf>& cur_state, cv::OutputArray observation) override
    {
        visual_obs_model_->observe(cur_state, observation);
    };

private:
    std::unique_ptr<VisualObservationModel> visual_obs_model_;
};

#endif /* VISUALCORRECTION_H */
