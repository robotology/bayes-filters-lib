#include "BayesFilters/PFCorrectionDecorator.h"

#include <utility>

using namespace bfl;
using namespace Eigen;


PFCorrectionDecorator::PFCorrectionDecorator(std::unique_ptr<PFCorrection> correction) noexcept :
    correction_(std::move(correction)) { }


PFCorrectionDecorator::PFCorrectionDecorator(PFCorrectionDecorator&& correction) noexcept :
    correction_(std::move(correction.correction_)) { }


PFCorrectionDecorator::~PFCorrectionDecorator() noexcept { }


PFCorrectionDecorator& PFCorrectionDecorator::operator=(PFCorrectionDecorator&& correction) noexcept
{
    correction_ = std::move(correction.correction_);

    return *this;
}


void PFCorrectionDecorator::correct(const Ref<const MatrixXf>& pred_states, const Ref<const VectorXf>& pred_weights, const Ref<const MatrixXf>& measurements,
                                    Ref<MatrixXf> cor_states, Ref<VectorXf> cor_weights)
{
    correction_->correct(pred_states, pred_weights, measurements,
                         cor_states, cor_weights);
}


void PFCorrectionDecorator::innovation(const Ref<const MatrixXf>& pred_states, const Ref<const MatrixXf>& measurements, Ref<MatrixXf> innovations)
{
    correction_->innovation(pred_states, measurements, innovations);
}


double PFCorrectionDecorator::likelihood(const Ref<const VectorXf>& innovation)
{
    return correction_->likelihood(innovation);
}


ObservationModel& PFCorrectionDecorator::getObservationModel()
{
    return correction_->getObservationModel();
}
