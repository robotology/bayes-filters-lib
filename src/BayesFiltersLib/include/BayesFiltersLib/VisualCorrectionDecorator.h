#ifndef VISUALCORRECTIONDECORATOR_H
#define VISUALCORRECTIONDECORATOR_H

#include "VisualCorrection.h"

#include <memory>

namespace bfl {
    class VisualCorrectionDecorator;
}


class bfl::VisualCorrectionDecorator : public VisualCorrection
{
public:
    virtual void correct(const Eigen::Ref<const Eigen::MatrixXf>& pred_state, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> cor_state) override;

    virtual void innovation(const Eigen::Ref<const Eigen::MatrixXf>& pred_state, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> innovation) override;

    virtual void likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovation, Eigen::Ref<Eigen::MatrixXf> cor_state) override;
    
protected:
    /* Default constructor, disabled */
    VisualCorrectionDecorator() = delete;

    /* Decorator constructor */
    VisualCorrectionDecorator(std::unique_ptr<VisualCorrection> visual_correction) noexcept;

    /* Destructor */
    ~VisualCorrectionDecorator() noexcept override;

    /* Move constructor */
    VisualCorrectionDecorator(VisualCorrectionDecorator&& visual_correction) noexcept;

    /* Move assignment operator */
    VisualCorrectionDecorator& operator=(VisualCorrectionDecorator&& visual_correction) noexcept;

private:
    std::unique_ptr<VisualCorrection> visual_correction_;
};

#endif /* VISUALCORRECTIONDECORATOR_H */
