#include "BayesFilters/PFVisualCorrection.h"

using namespace bfl;
using namespace cv;
using namespace Eigen;


PFVisualCorrection::PFVisualCorrection() noexcept { };


PFVisualCorrection::~PFVisualCorrection() noexcept { };


PFVisualCorrection::PFVisualCorrection(PFVisualCorrection&& pf_prediction) noexcept { }


void PFVisualCorrection::correct(const Ref<const MatrixXf>& pred_states, const Ref<const VectorXf>& pred_weights, InputArray measurements,
                                 Ref<MatrixXf> cor_states, Ref<VectorXf> cor_weights)
{
    if (!skip_)
        correctStep(pred_states, pred_weights, measurements,
                    cor_states, cor_weights);
    else
    {
        cor_states  = pred_states;
        cor_weights = pred_weights;
    }
}


bool PFVisualCorrection::skip(const bool status)
{
    skip_ = status;

    return true;
}
