#include <BayesFilters/MeasurementModel.h>

using namespace bfl;
using namespace Eigen;


std::pair<bool, MatrixXf> MeasurementModel::predictedMeasure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states)
{
    return std::make_pair(false, MatrixXf::Zero(1, 1));
}


std::pair<bool, MatrixXf> MeasurementModel::getNoiseSample(const int num)
{
    return std::make_pair(false, MatrixXf::Zero(1, 1));
}


std::pair<bool, MatrixXf> MeasurementModel::getNoiseCovarianceMatrix()
{
    return std::make_pair(false, MatrixXf::Zero(1, 1));
}


bool MeasurementModel::setProperty(const std::string property)
{
    return false;
}
