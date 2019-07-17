/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/LinearModel.h>

#include <cmath>
#include <iostream>
#include <utility>

#include <Eigen/Cholesky>

using namespace bfl;
using namespace Eigen;


LinearModel::LinearModel
(
    const LinearMatrixComponent& linear_matrix_component,
    const Ref<const MatrixXd>& noise_covariance_matrix,
    const unsigned int seed
) :
    LTIMeasurementModel(MatrixXd::Zero(linear_matrix_component.second.size(), linear_matrix_component.first), noise_covariance_matrix),
    generator_(std::mt19937_64(seed)),
    distribution_(std::normal_distribution<double>(0.0, 1.0)),
    gauss_rnd_sample_([&] { return (distribution_)(generator_); })
{
    for (std::size_t i = 0; i < linear_matrix_component.second.size(); ++i)
    {
        const std::size_t& component_index = linear_matrix_component.second[i];

        if (component_index < linear_matrix_component.first)
            H_(i, component_index) = 1.0;
        else
            throw std::runtime_error(std::string("ERROR::LINEARMODEL::CTOR\nERROR:\n\tIndex component out of bound.\nLOG:\n\tProvided: ") + std::to_string(component_index) + ". Index bound: " + std::to_string(linear_matrix_component.first) + ".");
    }

    LDLT<MatrixXd> chol_ldlt(R_);
    sqrt_R_ = (chol_ldlt.transpositionsP() * MatrixXd::Identity(R_.rows(), R_.cols())).transpose() * chol_ldlt.matrixL() * chol_ldlt.vectorD().real().cwiseSqrt().asDiagonal();
}


LinearModel::LinearModel(const LinearMatrixComponent& linear_matrix_component, const Ref<const MatrixXd>& noise_covariance_matrix) :
    LinearModel(linear_matrix_component, noise_covariance_matrix, 1)
{ }


std::pair<bool, MatrixXd> LinearModel::getNoiseSample(const int num) const
{
    MatrixXd rand_vectors(2, num);
    for (int i = 0; i < rand_vectors.size(); i++)
        *(rand_vectors.data() + i) = gauss_rnd_sample_();

    MatrixXd noise_sample = sqrt_R_ * rand_vectors;

    return std::make_pair(true, std::move(noise_sample));
}


std::pair<bool, MatrixXd> LinearModel::getNoiseCovarianceMatrix() const
{
    return std::make_pair(true, R_);
}


Eigen::MatrixXd LinearModel::getMeasurementMatrix() const
{
    return H_;
}
