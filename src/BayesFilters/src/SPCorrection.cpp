#include "BayesFilters/SPCorrection.h"

using namespace bfl;
using namespace Eigen;


SPCorrection::SPCorrection() noexcept { };


SPCorrection::SPCorrection(SPCorrection&& pf_prediction) noexcept { }


Gaussian SPCorrection::correct(const Gaussian& pred_state, const Ref<const MatrixXf>& measurements)
{
    if (!skip_)
        return correctStep(pred_state, measurements);
    else
        return pred_state;
}


bool SPCorrection::skip(const bool status)
{
    skip_ = status;

    return true;
}


