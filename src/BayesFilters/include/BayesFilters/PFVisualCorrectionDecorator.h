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
    void correct(Eigen::Ref<Eigen::MatrixXf> states, Eigen::Ref<Eigen::MatrixXf> weights, cv::InputArray measurements) override;

    void innovation(const Eigen::Ref<const Eigen::MatrixXf>& states, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> innovations) override;

    void likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations, Eigen::Ref<Eigen::MatrixXf> weights) override;

    void observeState(const Eigen::Ref<const Eigen::MatrixXf>& states, cv::OutputArray observations) override;

    void measureState(const Eigen::Ref<const Eigen::MatrixXf>& states, cv::OutputArray measurements) override;

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
