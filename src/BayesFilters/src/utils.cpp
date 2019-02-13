#include <BayesFilters/utils.h>

using namespace Eigen;


double bfl::utils::log_sum_exp(const Ref<const VectorXd>& arguments)
{
    double max = arguments.maxCoeff();

    return max + log((arguments.array() - max).exp().sum());
}
