#include <BayesFilters/LTIMeasurementModel.h>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


LTIMeasurementModel::LTIMeasurementModel(const Ref<const MatrixXf>& measurement_matrix, const Ref<const MatrixXf>& noise_covariance_matrix)
    : H_(measurement_matrix), R_(noise_covariance_matrix)
{
    if ((H_.rows() == 0) || (H_.cols() == 0))
        throw std::runtime_error("ERROR::LTIMEASUREMENTMODEL::CTOR\nERROR:\n\tMeasurement matrix dimensions cannot be 0.");
    else if ((R_.rows() == 0) || (R_.cols() == 0))
        throw std::runtime_error("ERROR::LTIMEASUREMENTMODEL::CTOR\nERROR:\n\tNoise covariance matrix dimensions cannot be 0.");
    else if (R_.rows() != R_.cols())
        throw std::runtime_error("ERROR::LTIMEASUREMENTMODEL::CTOR\nERROR:\n\tNoise covariance matrix must be a square matrix.");
    else if (H_.rows() != R_.rows())
        throw std::runtime_error("ERROR::LTIMEASUREMENTMODEL::CTOR\nERROR:\n\tNumber of rows of the measurement matrix must be the same as the size of the noise covariance matrix.");
}


std::pair<bool, Eigen::MatrixXf> LTIMeasurementModel::getNoiseCovarianceMatrix() const
{
    return std::make_pair(true, R_);
}


Eigen::MatrixXf LTIMeasurementModel::getMeasurementMatrix() const
{
    return H_;
}
