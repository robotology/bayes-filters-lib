#include <BayesFilters/MeasurementModel.h>

using namespace bfl;
using namespace Eigen;


bool MeasurementModel::bufferProcessMeasurements()
{
    return false;
}


std::pair<bool, MatrixXf> MeasurementModel::getProcessMeasurements() const
{
    return std::make_pair(false, MatrixXf::Zero(1, 1));
}


std::pair<bool, MatrixXf> MeasurementModel::getNoiseSample(const int num) const
{
    return std::make_pair(false, MatrixXf::Zero(1, 1));
}


std::pair<bool, MatrixXf> MeasurementModel::getNoiseCovarianceMatrix() const
{
    return std::make_pair(false, MatrixXf::Zero(1, 1));
}


bool MeasurementModel::setProperty(const std::string property)
{
    return false;
}
