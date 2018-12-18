#include <BayesFilters/MeasurementModel.h>

#include <iostream>

using namespace bfl;
using namespace Eigen;


MeasurementModel::~MeasurementModel() noexcept
{ }


std::pair<bool, MatrixXd> MeasurementModel::getNoiseCovarianceMatrix() const
{
    return std::make_pair(false, MatrixXd::Zero(1, 1));
}


bool MeasurementModel::setProperty(const std::string& property)
{
    static_cast<void>(property);
    return false;
}
