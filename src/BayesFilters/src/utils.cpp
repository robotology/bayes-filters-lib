/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/utils.h>

using namespace Eigen;


double bfl::utils::log_sum_exp(const Ref<const VectorXd>& arguments)
{
    double max = arguments.maxCoeff();

    return max + log((arguments.array() - max).exp().sum());
}


MatrixXd bfl::utils::quaternion_to_rotation_vector(const Ref<const MatrixXd>& quaternion)
{
    MatrixXd rotation_vectors(3, quaternion.cols());
    for (std::size_t i = 0; i < quaternion.size(); ++i)
    {
        const double norm_n = quaternion.col(i).tail<3>().norm();
        if (norm_n > 0)
        {
            const double w = quaternion.col(i)(0);
            if (w < 0)
                rotation_vectors.col(i) = - 2.0 * std::acos(-w) * quaternion.col(i).tail<3>() / norm_n;
            else
                rotation_vectors.col(i) = 2.0 * std::acos(w) * quaternion.col(i).tail<3>() / norm_n;
        }
        else
            rotation_vectors.col(i) = Vector3d::Zero();
    }

    return rotation_vectors;
}


MatrixXd bfl::utils::rotation_vector_to_quaternion(const Ref<const MatrixXd>& rotation_vector)
{
    MatrixXd quaternions(4, rotation_vector.cols());
    for (std::size_t i = 0; i < rotation_vector.cols(); ++i)
    {
        const double norm_r = rotation_vector.col(i).norm();
        if (norm_r > 0)
        {
            quaternions.col(i)(0) = std::cos(norm_r / 2.0);
            quaternions.col(i).tail<3>() = std::sin(norm_r / 2.0) * rotation_vector.col(i) / norm_r;
        }
        else
        {
            quaternions.col(i)(0) = 1.0;
            quaternions.col(i).tail<3>() = Vector3d::Zero();
        }
    }

    return quaternions;
}


MatrixXd bfl::utils::sum_quaternion_rotation_vector(const Ref<const MatrixXd>& quaternion, const Ref<const MatrixXd>& rotation_vector)
{
    /* Move rotation vectors to their quaternionic representation. */
    MatrixXd vector_as_quaternion = rotation_vector_to_quaternion(rotation_vector);

    Quaterniond q_right(quaternion.col(0)(0), quaternion.col(0)(1), quaternion.col(0)(2), quaternion.col(0)(3));

    MatrixXd quaternions(4, rotation_vector.cols());
    for (std::size_t i = 0; i < rotation_vector.cols(); ++i)
    {
        Quaterniond q_left(vector_as_quaternion.col(i)(0), vector_as_quaternion.col(i)(1), vector_as_quaternion.col(i)(2), vector_as_quaternion.col(i)(3));
        Quaterniond sum = q_left * q_right;

        quaternions.col(i)(0) = sum.w();
        quaternions.col(i).tail<3>() = sum.vec();
    }

    return quaternions;
}


MatrixXd bfl::utils::diff_quaternion(const Ref<const MatrixXd>& quaternion_left, const Ref<const MatrixXd>& quaternion_right)
{
    MatrixXd products(4, quaternion_left.cols());

    Quaterniond q_right(quaternion_right.col(0)(0), quaternion_right.col(0)(1), quaternion_right.col(0)(2), quaternion_right.col(0)(3));
    Quaterniond q_right_conj = q_right.conjugate();

    /* Products between each left quaternion and the conjugated right quaternion. */
    for (std::size_t i = 0; i < quaternion_left.cols(); ++i)
    {
        Quaterniond q_left(quaternion_left.col(i)(0), quaternion_left.col(i)(1), quaternion_left.col(i)(2), quaternion_left.col(i)(3));
        Quaterniond product = q_left * q_right_conj;

        products.col(i)(0) = product.w();
        products.col(i).tail<3>() = product.vec();
    }

    /* Express displacements in the tangent space. */
    return quaternion_to_rotation_vector(products);
}


VectorXd bfl::utils::mean_quaternion(const Ref<const MatrixXd>& weight, const Ref<const MatrixXd>& quaternion)
{
    /* Weighted outer product of quaternions. */
    MatrixXd outer_product_mean = Matrix4d::Zero();
    for (std::size_t i = 0; i < weight.rows(); ++i)
        outer_product_mean.noalias() += weight.col(0)(i) * quaternion.col(i) * quaternion.col(i).transpose();

    /* Take the weighted mean as the eigenvector corresponding to the maximum eigenvalue. */
    EigenSolver<Matrix4d> eigen_solver(outer_product_mean);
    Matrix<std::complex<double>, 4, 1> eigenvalues(eigen_solver.eigenvalues());
    int maximum_index;
    eigenvalues.real().maxCoeff(&maximum_index);
    return eigen_solver.eigenvectors().real().block(0, maximum_index, 4, 1);
}
