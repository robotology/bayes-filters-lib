#include <BayesFilters/LinearStateModel.h>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


void LinearStateModel::propagate(const Eigen::Ref<const Eigen::MatrixXf>& cur_states, Eigen::Ref<Eigen::MatrixXf> pred_states)
{
    pred_states = getStateTransitionMatrix() * cur_states;
}
