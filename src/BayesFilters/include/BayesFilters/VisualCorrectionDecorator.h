#ifndef VISUALCORRECTIONDECORATOR_H
#define VISUALCORRECTIONDECORATOR_H

#include "AbstractVisualCorrection.h"

#include <memory>

namespace bfl {
    class VisualCorrectionDecorator;
}


class bfl::VisualCorrectionDecorator : public AbstractVisualCorrection
{
public:
    void correct(Eigen::Ref<Eigen::MatrixXf> states, Eigen::Ref<Eigen::MatrixXf> weights, cv::InputArray measurements) override;

    void innovation(const Eigen::Ref<const Eigen::MatrixXf>& states, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> innovations) override;

    void likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations, Eigen::Ref<Eigen::MatrixXf> weights) override;

    bool setModelProperty(const std::string& property) override;

    void observeState(const Eigen::Ref<const Eigen::MatrixXf>& states, cv::OutputArray observations) override;

    void measureState(const Eigen::Ref<const Eigen::MatrixXf>& states, cv::OutputArray measurements) override;
    
protected:
    VisualCorrectionDecorator(std::unique_ptr<AbstractVisualCorrection> visual_correction) noexcept;

    VisualCorrectionDecorator(VisualCorrectionDecorator&& visual_correction) noexcept;

    ~VisualCorrectionDecorator() noexcept override;

    VisualCorrectionDecorator& operator=(VisualCorrectionDecorator&& visual_correction) noexcept;

private:
    std::unique_ptr<AbstractVisualCorrection> visual_correction_;
};

#endif /* VISUALCORRECTIONDECORATOR_H */
