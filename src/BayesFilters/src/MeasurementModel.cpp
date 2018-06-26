#include <BayesFilters/MeasurementModel.h>

#include <iostream>

using namespace bfl;
using namespace Eigen;


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


bool MeasurementModel::bufferProcessMeasurements()
{
    return false;
}


std::pair<bool, MatrixXf> MeasurementModel::getProcessMeasurements() const
{
    return std::make_pair(false, MatrixXf::Zero(1, 1));
}


void MeasurementModel::enableLog(const std::string& prefix_name)
{
    static_cast<void>(prefix_name);
    std::cerr << "WARNING::MEASUREMENTMODEL::ENABLELOG\n";
    std::cerr << "\tWARNING: Log facility is not implemented for the MeasurementModel class. Nothing to enable." << std::endl;
}


void MeasurementModel::disableLog()
{
    std::cerr << "WARNING::MEASUREMENTMODEL::DISABLELOG\n";
    std::cerr << "\tWARNING: Log facility is not implemented for the MeasurementModel class. Nothing to disable." << std::endl;
}