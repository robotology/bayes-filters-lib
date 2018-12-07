#include <BayesFilters/StateModel.h>

using namespace bfl;
using namespace Eigen;


Eigen::MatrixXd StateModel::getJacobian()
{
    throw std::runtime_error("ERROR::STATEMODEL::GETJACOBIAN\nERROR:\n\tMethod not implemented.");
}


Eigen::VectorXd StateModel::getTransitionProbability(const Eigen::Ref<const Eigen::MatrixXd>& prev_states, Eigen::Ref<Eigen::MatrixXd> cur_states)
{
    throw std::runtime_error("ERROR::STATEMODEL::TRANSITIONPROBABILITY\nERROR:\n\tMethod not implemented.");
}
