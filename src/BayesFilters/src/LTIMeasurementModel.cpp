/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/LTIMeasurementModel.h>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


LTIMeasurementModel::LTIMeasurementModel(const Ref<const MatrixXd>& measurement_matrix, const Ref<const MatrixXd>& noise_covariance_matrix)
{
    if ((measurement_matrix.rows() == 0) || (measurement_matrix.cols() == 0))
        throw std::runtime_error("ERROR::LTIMEASUREMENTMODEL::CTOR\nERROR:\n\tMeasurement matrix dimensions cannot be 0.");
    else if ((noise_covariance_matrix.rows() == 0) || (noise_covariance_matrix.cols() == 0))
        throw std::runtime_error("ERROR::LTIMEASUREMENTMODEL::CTOR\nERROR:\n\tNoise covariance matrix dimensions cannot be 0.");
    else if (noise_covariance_matrix.rows() != noise_covariance_matrix.cols())
        throw std::runtime_error("ERROR::LTIMEASUREMENTMODEL::CTOR\nERROR:\n\tNoise covariance matrix must be a square matrix.");
    else if (measurement_matrix.rows() != noise_covariance_matrix.rows())
        throw std::runtime_error("ERROR::LTIMEASUREMENTMODEL::CTOR\nERROR:\n\tNumber of rows of the measurement matrix must be the same as the size of the noise covariance matrix.");

    H_ = measurement_matrix;
    R_ = noise_covariance_matrix;
}


std::pair<bool, Eigen::MatrixXd> LTIMeasurementModel::getNoiseCovarianceMatrix() const
{
    return std::make_pair(true, R_);
}


Eigen::MatrixXd LTIMeasurementModel::getMeasurementMatrix() const
{
    return H_;
}
