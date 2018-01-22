#include "BayesFilters/SPCorrection.h"

using namespace bfl;
using namespace Eigen;


SPCorrection::SPCorrection() noexcept { };


SPCorrection::SPCorrection(SPCorrection&& pf_prediction) noexcept { }


void SPCorrection::correct(const Ref<const MatrixXf>& pred_states, const std::vector<MatrixXf>& pred_covariances, const Ref<const MatrixXf>& measurements,
                           Ref<MatrixXf> cor_states, std::vector<MatrixXf>& cor_covariances)
{
    if (!skip_)
        correctStep(pred_states, pred_covariances, measurements,
                    cor_states, cor_covariances);
    else
    {
        cor_states  = pred_states;
        cor_covariances = pred_covariances;
    }
}


bool SPCorrection::skip(const bool status)
{
    skip_ = status;

    return true;
}


