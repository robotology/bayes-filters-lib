#include "BayesFilters/PFVisualCorrectionDecorator.h"

using namespace bfl;
using namespace cv;
using namespace Eigen;


PFVisualCorrectionDecorator::PFVisualCorrectionDecorator(std::unique_ptr<PFVisualCorrection> visual_correction) noexcept :
visual_correction_(std::move(visual_correction)) { }


PFVisualCorrectionDecorator::PFVisualCorrectionDecorator(PFVisualCorrectionDecorator&& visual_correction) noexcept :
visual_correction_(std::move(visual_correction.visual_correction_)) { }


PFVisualCorrectionDecorator::~PFVisualCorrectionDecorator() noexcept { }


PFVisualCorrectionDecorator& PFVisualCorrectionDecorator::operator=(PFVisualCorrectionDecorator&& visual_correction) noexcept
{
    visual_correction_ = std::move(visual_correction.visual_correction_);

    return *this;
}

void PFVisualCorrectionDecorator::correct(Ref<MatrixXf> states, Ref<MatrixXf> weights, cv::InputArray measurements)
{
    visual_correction_->correct(states, weights, measurements);
}


void PFVisualCorrectionDecorator::innovation(const Ref<const MatrixXf>& states, InputArray measurements, Ref<MatrixXf> innovations)
{
    visual_correction_->innovation(states, measurements, innovations);
}


void PFVisualCorrectionDecorator::likelihood(const Ref<const MatrixXf>& innovations, Ref<MatrixXf> weights)
{
    visual_correction_->likelihood(innovations, weights);
}


void PFVisualCorrectionDecorator::observeState(const Ref<const MatrixXf>& states, cv::OutputArray observations)
{
    return visual_correction_->observeState(states, observations);
}


void PFVisualCorrectionDecorator::measureState(const Eigen::Ref<const Eigen::MatrixXf>& states, cv::OutputArray measurements)
{
    return visual_correction_->measureState(states, measurements);
}


VisualObservationModel& PFVisualCorrectionDecorator::getVisualObservationModel()
{
    return visual_correction_->getVisualObservationModel();
}
