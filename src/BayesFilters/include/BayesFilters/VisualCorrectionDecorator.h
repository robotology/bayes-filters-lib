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
    void correct(const Eigen::Ref<const Eigen::MatrixXf>& pred_state, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> cor_state) override;

    void innovation(const Eigen::Ref<const Eigen::MatrixXf>& pred_state, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> innovation) override;

    void likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovation, Eigen::Ref<Eigen::MatrixXf> cor_state) override;

    bool setObservationModelProperty(const std::string& property) override;

    void observe(const Eigen::Ref<const Eigen::MatrixXf>& cur_state, cv::OutputArray observation) override;
    
protected:
    /* Constructor */
    VisualCorrectionDecorator(std::unique_ptr<AbstractVisualCorrection> visual_correction) noexcept;

    /* Destructor */
    ~VisualCorrectionDecorator() noexcept override;

    /* Move constructor */
    VisualCorrectionDecorator(VisualCorrectionDecorator&& visual_correction) noexcept;

    /* Move assignment operator */
    VisualCorrectionDecorator& operator=(VisualCorrectionDecorator&& visual_correction) noexcept;

private:
    std::unique_ptr<AbstractVisualCorrection> visual_correction_;
};

#endif /* VISUALCORRECTIONDECORATOR_H */
