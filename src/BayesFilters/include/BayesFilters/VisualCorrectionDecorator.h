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
    virtual void correct(Eigen::Ref<Eigen::MatrixXf> states, Eigen::Ref<Eigen::MatrixXf> weights, cv::InputArray measurements) override;

    virtual void innovation(const Eigen::Ref<const Eigen::MatrixXf>& states, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> innovations) override;

    virtual void likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations, Eigen::Ref<Eigen::MatrixXf> weights) override;

    virtual bool setObservationModelProperty(const std::string& property) override;

    virtual void observe(const Eigen::Ref<const Eigen::MatrixXf>& states, cv::OutputArray observations) override;
    
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
