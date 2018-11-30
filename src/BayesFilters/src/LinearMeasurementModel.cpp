#include <BayesFilters/LinearMeasurementModel.h>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


std::pair<bool, bfl::Data> LinearMeasurementModel::predictedMeasure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) const
{
    MatrixXf prediction = getMeasurementMatrix() * cur_states;
    return std::make_pair(true, std::move(prediction));
}


std::pair<bool, bfl::Data> LinearMeasurementModel::innovation(const bfl::Data& predicted_measurements, const bfl::Data& measurements) const
{
    MatrixXf innovation = -(any::any_cast<MatrixXf>(predicted_measurements).colwise() - any::any_cast<MatrixXf>(measurements).col(0));

    return std::make_pair(true, std::move(innovation));
}
