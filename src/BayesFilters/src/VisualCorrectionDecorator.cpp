#include "BayesFilters/VisualCorrectionDecorator.h"

using namespace bfl;
using namespace cv;
using namespace Eigen;


void VisualCorrectionDecorator::correct(const Ref<const MatrixXf>& pred_state, InputArray measurements, Ref<MatrixXf> cor_state)
{
    visual_correction_->correct(pred_state, measurements, cor_state);
}


void VisualCorrectionDecorator::innovation(const Ref<const MatrixXf>& pred_state, InputArray measurements, Ref<MatrixXf> innovation)
{
    visual_correction_->innovation(pred_state, measurements, innovation);
}


void VisualCorrectionDecorator::likelihood(const Ref<const MatrixXf>& innovation, Ref<MatrixXf> cor_state)
{
    visual_correction_->likelihood(innovation, cor_state);
}


bool VisualCorrectionDecorator::setObservationModelProperty(const std::string& property)
{
    return visual_correction_->setObservationModelProperty(property);
}


void VisualCorrectionDecorator::observe(const Eigen::Ref<const Eigen::MatrixXf>& cur_state, cv::OutputArray observation)
{
    return visual_correction_->observe(cur_state, observation);
}


VisualCorrectionDecorator::VisualCorrectionDecorator(std::unique_ptr<AbstractVisualCorrection> visual_correction) noexcept :
    visual_correction_(std::move(visual_correction)) { }


VisualCorrectionDecorator::~VisualCorrectionDecorator() noexcept { }


VisualCorrectionDecorator::VisualCorrectionDecorator(VisualCorrectionDecorator&& visual_correction) noexcept :
    visual_correction_(std::move(visual_correction.visual_correction_)) { }


VisualCorrectionDecorator& VisualCorrectionDecorator::operator=(VisualCorrectionDecorator&& visual_correction) noexcept
{
    visual_correction_ = std::move(visual_correction.visual_correction_);

    return *this;
}
