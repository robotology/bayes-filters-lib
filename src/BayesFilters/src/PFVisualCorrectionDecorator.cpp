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

void PFVisualCorrectionDecorator::correct(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights, cv::InputArray measurements,
                                          Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights)
{
    visual_correction_->correct(pred_states, pred_weights, measurements,
                                cor_states, cor_weights);
}


void PFVisualCorrectionDecorator::innovation(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, cv::InputArray measurements, Eigen::Ref<Eigen::MatrixXf> innovations)
{
    visual_correction_->innovation(pred_states, measurements, innovations);
}


double PFVisualCorrectionDecorator::likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations)
{
    return visual_correction_->likelihood(innovations);
}


VisualObservationModel& PFVisualCorrectionDecorator::getVisualObservationModel()
{
    return visual_correction_->getVisualObservationModel();
}
