#include <BayesFilters/PFCorrection.h>

using namespace bfl;
using namespace Eigen;


PFCorrection::PFCorrection() noexcept { };


void PFCorrection::correct(const Ref<const MatrixXf>& pred_states, const Ref<const VectorXf>& pred_weights,
                           Ref<MatrixXf> cor_states, Ref<VectorXf> cor_weights)
{
    if (!skip_)
        correctStep(pred_states, pred_weights,
                    cor_states, cor_weights);
    else
    {
        cor_states  = pred_states;
        cor_weights = pred_weights;
    }
}


bool PFCorrection::skip(const bool status)
{
    skip_ = status;

    return true;
}
