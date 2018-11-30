#include <BayesFilters/StateModel.h>

using namespace bfl;
using namespace Eigen;


Eigen::MatrixXf StateModel::getJacobian()
{
    throw std::runtime_error("ERROR::STATEMODEL::GETJACOBIAN\nERROR:\n\tMethod not implemented.");
}


Eigen::VectorXf StateModel::getTransitionProbability(const Eigen::Ref<const Eigen::MatrixXf>& prev_states, Eigen::Ref<Eigen::MatrixXf> cur_states)
{
    throw std::runtime_error("ERROR::STATEMODEL::TRANSITIONPROBABILITY\nERROR:\n\tMethod not implemented.");
}
