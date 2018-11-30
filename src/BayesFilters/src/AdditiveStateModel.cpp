#include <BayesFilters/AdditiveStateModel.h>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;

void AdditiveStateModel::motion
(
    const Eigen::Ref<const Eigen::MatrixXf>& cur_states,
    Eigen::Ref<Eigen::MatrixXf> mot_states
)
{
    propagate(cur_states, mot_states);
    mot_states += getNoiseSample(mot_states.cols());
}
