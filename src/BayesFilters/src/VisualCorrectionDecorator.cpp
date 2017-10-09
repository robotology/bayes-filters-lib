#include "BayesFilters/VisualCorrectionDecorator.h"

using namespace bfl;
using namespace cv;
using namespace Eigen;


VisualCorrectionDecorator::VisualCorrectionDecorator(std::unique_ptr<AbstractVisualCorrection> visual_correction) noexcept :
visual_correction_(std::move(visual_correction)) { }


VisualCorrectionDecorator::VisualCorrectionDecorator(VisualCorrectionDecorator&& visual_correction) noexcept :
visual_correction_(std::move(visual_correction.visual_correction_)) { }


VisualCorrectionDecorator::~VisualCorrectionDecorator() noexcept { }


VisualCorrectionDecorator& VisualCorrectionDecorator::operator=(VisualCorrectionDecorator&& visual_correction) noexcept
{
    visual_correction_ = std::move(visual_correction.visual_correction_);

    return *this;
}

void VisualCorrectionDecorator::correct(Ref<MatrixXf> states, Ref<MatrixXf> weights, cv::InputArray measurements)
{
    visual_correction_->correct(states, weights, measurements);
}


void VisualCorrectionDecorator::innovation(const Ref<const MatrixXf>& states, InputArray measurements, Ref<MatrixXf> innovations)
{
    visual_correction_->innovation(states, measurements, innovations);
}


void VisualCorrectionDecorator::likelihood(const Ref<const MatrixXf>& innovations, Ref<MatrixXf> weights)
{
    visual_correction_->likelihood(innovations, weights);
}


bool VisualCorrectionDecorator::setModelProperty(const std::string& property)
{
    return visual_correction_->setModelProperty(property);
}


void VisualCorrectionDecorator::observeState(const Ref<const MatrixXf>& states, cv::OutputArray observations)
{
    return visual_correction_->observeState(states, observations);
}


void VisualCorrectionDecorator::measureState(const Eigen::Ref<const Eigen::MatrixXf>& states, cv::OutputArray measurements)
{
    return visual_correction_->measureState(states, measurements);
}
