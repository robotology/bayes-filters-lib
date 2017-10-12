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
    void correct(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights, cv::InputArray measurements,
                 Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) override;

    void innovation(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> innovations) override;

    void likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations,const Eigen::Ref<const Eigen::VectorXf>& pred_weights,
                    Eigen::Ref<Eigen::VectorXf>& likelihoods) override;

    VisualObservationModel& getVisualObservationModel() override;
    
protected:
    PFVisualCorrectionDecorator(std::unique_ptr<PFVisualCorrection> visual_correction) noexcept;

    PFVisualCorrectionDecorator(PFVisualCorrectionDecorator&& visual_correction) noexcept;

    ~PFVisualCorrectionDecorator() noexcept override;

    PFVisualCorrectionDecorator& operator=(PFVisualCorrectionDecorator&& visual_correction) noexcept;

private:
    std::unique_ptr<PFVisualCorrection> visual_correction_;
};

#endif /* PFVISUALCORRECTIONDECORATOR_H */
