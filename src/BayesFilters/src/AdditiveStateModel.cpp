#include <BayesFilters/AdditiveStateModel.h>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;

void AdditiveStateModel::motion(const Eigen::Ref<const Eigen::MatrixXd>& cur_states, Eigen::Ref<Eigen::MatrixXd> mot_states)
{
    propagate(cur_states, mot_states);
    mot_states += getNoiseSample(mot_states.cols());
}
