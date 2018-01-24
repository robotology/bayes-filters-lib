#ifndef PFVISUALCORRECTIONDECORATOR_H
#define PFVISUALCORRECTIONDECORATOR_H

#include "PFVisualCorrection.h"

#include <memory>

namespace bfl {
    class PFVisualCorrectionDecorator;
}


class bfl::PFVisualCorrectionDecorator : public PFVisualCorrection
{
public:
    void innovation(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> innovations) override;

    double likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations) override;

    VisualObservationModel& getVisualObservationModel() override;

    void setVisualObservationModel(std::unique_ptr<VisualObservationModel> visual_observation_model) override;
    
protected:
    PFVisualCorrectionDecorator(std::unique_ptr<PFVisualCorrection> visual_correction) noexcept;

    PFVisualCorrectionDecorator(PFVisualCorrectionDecorator&& visual_correction) noexcept;

    virtual ~PFVisualCorrectionDecorator() noexcept;

    void correctStep(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights, cv::InputArray measurements,
                     Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) override;

private:
    std::unique_ptr<PFVisualCorrection> visual_correction_;
};

#endif /* PFVISUALCORRECTIONDECORATOR_H */
