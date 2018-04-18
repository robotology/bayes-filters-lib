#include "BayesFilters/PFCorrectionDecorator.h"

#include <utility>

using namespace bfl;
using namespace Eigen;


PFCorrectionDecorator::PFCorrectionDecorator(std::unique_ptr<PFCorrection> correction) noexcept :
    correction_(std::move(correction)) { }


PFCorrectionDecorator::PFCorrectionDecorator(PFCorrectionDecorator&& correction) noexcept :
    correction_(std::move(correction.correction_)) { }


PFCorrectionDecorator::~PFCorrectionDecorator() noexcept { }


VectorXf PFCorrectionDecorator::getLikelihood()
{
    return correction_->getLikelihood();
}


void PFCorrectionDecorator::correctStep(const Ref<const MatrixXf>& pred_states, const Ref<const VectorXf>& pred_weights,
                                        Ref<MatrixXf> cor_states, Ref<VectorXf> cor_weights)
{
    correction_->correctStep(pred_states, pred_weights,
                             cor_states, cor_weights);
}
