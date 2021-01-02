/*
 * Copyright (C) 2016-2020 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <iostream>

#include <BayesFilters/utils.h>

using namespace bfl;
using namespace Eigen;


int main()
{
    /* A reference rotation vector with small numbers. */
    Vector3d rot_vec_0;
    rot_vec_0 << 0.001, 0.002, 0.003;

    Quaterniond quat_0(AngleAxisd(rot_vec_0.norm(), rot_vec_0.normalized()));

    /* A reference rotation vector with not small numbers. */
    Vector3d rot_vec_1;
    rot_vec_1 << 1.1, 1.2, 1.3;

    Quaterniond quat_1(AngleAxisd(rot_vec_1.norm(), rot_vec_1.normalized()));

    std::cout << "Test quaternion_to_rotation_vector...\n" << std::endl;
    {
        MatrixXd q(4, 2);

        q.col(0)(0) = quat_0.w();
        q.col(0)(1) = quat_0.x();
        q.col(0)(2) = quat_0.y();
        q.col(0)(3) = quat_0.z();

        q.col(1)(0) = quat_1.w();
        q.col(1)(1) = quat_1.x();
        q.col(1)(2) = quat_1.y();
        q.col(1)(3) = quat_1.z();

        auto r = utils::quaternion_to_rotation_vector(q);

        std::cout << "quaternion_to_rotation_vector produced output with " << r.rows() << " rows (should be 3)" << std::endl;
        if (r.rows() != 3)
            return EXIT_FAILURE;

        std::cout << "quaternion_to_rotation_vector produced output with " << r.cols() << " cols (should be 2)" << std::endl;
        if (r.cols() != 2)
            return EXIT_FAILURE;

        std::cout << "The first column is " << r.col(0).transpose() << " (should be " << rot_vec_0.transpose() << ")" << std::endl;
        if (!(r.col(0).isApprox(rot_vec_0, 1e-5)))
            return EXIT_FAILURE;

        std::cout << "The second column is " << r.col(1).transpose() << " (should be " << rot_vec_1.transpose() << ")" << std::endl;
        if (!(r.col(1).isApprox(rot_vec_1, 1e-5)))
            return EXIT_FAILURE;
    }

    std::cout << "Test rotation_vector_to_quaternion...\n" << std::endl;
    {
        MatrixXd r(3, 2);

        r.col(0) = rot_vec_0;
        r.col(1) = rot_vec_1;

        auto q = utils::rotation_vector_to_quaternion(r);

        std::cout << "rotation_vector_to_quaternion produced output with " << q.rows() << " rows (should be 4)" << std::endl;
        if (q.rows() != 4)
            return EXIT_FAILURE;

        std::cout << "rotation_vector_to_quaternion produced output with " << q.cols() << " cols (should be 2)" << std::endl;
        if (q.cols() != 2)
            return EXIT_FAILURE;

        std::cout << "The first column is " << q.col(0).transpose() << " (should be " << quat_0.w() << " " << quat_0.vec().transpose() << ")" << std::endl;
        if (!(Quaterniond(q.col(0)(0), q.col(0)(1), q.col(0)(2), q.col(0)(3)).isApprox(quat_0, 1e-5)))
            return EXIT_FAILURE;

        std::cout << "The second column is " << q.col(1).transpose() << " (should be " << quat_1.w() << " " << quat_1.vec().transpose() << ")" << std::endl;
        if (!(Quaterniond(q.col(1)(0), q.col(1)(1), q.col(1)(2), q.col(1)(3)).isApprox(quat_1, 1e-5)))
            return EXIT_FAILURE;
    }

    std::cout << "Test sum_quaternion_rotation_vector...\n" << std::endl;
    {
        Quaterniond quat_sum_0 = quat_0;

        /* Note: summation in utils::sum_quaternion_rotation_vector applies a rotation vector in inertial axes which corresponds to pre-multiplication. */
        Quaterniond quat_sum_1 = quat_1 * quat_0;

        MatrixXd q(4, 1);

        q.col(0)(0) = quat_0.w();
        q.col(0)(1) = quat_0.x();
        q.col(0)(2) = quat_0.y();
        q.col(0)(3) = quat_0.z();

        MatrixXd r(3, 2);

        r.col(0) = Vector3d::Zero();
        r.col(1) = rot_vec_1;

        auto sum = utils::sum_quaternion_rotation_vector(q, r);

        std::cerr << "sum_quaternion_rotation_vector produced output with " << sum.rows() << " rows (should be 4)" << std::endl;
        if (sum.rows() != 4)
            return EXIT_FAILURE;

        std::cerr << "sum_quaternion_rotation_vector produced output with " << sum.cols() << " cols (should be 2)" << std::endl;
        if (sum.cols() != 2)
            return EXIT_FAILURE;

        std::cerr << "The first column is " << sum.col(0).transpose() << " (should be " << quat_sum_0.w() << " " << quat_sum_0.vec().transpose() << ")" << std::endl;
        if (!(Quaterniond(sum.col(0)(0), sum.col(0)(1), sum.col(0)(2), sum.col(0)(3)).isApprox(quat_sum_0, 1e-5)))
            return EXIT_FAILURE;

        std::cerr << "The second column is " << sum.col(1).transpose() << " (should be " << quat_sum_1.w() << " " << quat_sum_1.vec().transpose() << ")" << std::endl;
        if (!(Quaterniond(sum.col(1)(0), sum.col(1)(1), sum.col(1)(2), sum.col(1)(3)).isApprox(quat_sum_1, 1e-5)))
            return EXIT_FAILURE;
    }

    std::cout << "Test diff_quaternion...\n" << std::endl;
    {
        Vector3d rot_diff_0 = Vector3d::Zero();

        /**
         * Note: difference in utils::diff_quaternion post-multiplies the left operand with the conjugate of the right operand such that difference * right_operand = left_operand.
         * In our case we will use left_operand = quat_1 and right_operand = quat_0.
         */
        Quaterniond quat_diff_1 = quat_1 * quat_0.conjugate();
        AngleAxisd aa_diff_1 = AngleAxisd(quat_diff_1);
        Vector3d rot_diff_1 = aa_diff_1.angle() * aa_diff_1.axis();

        MatrixXd q_left(4, 2);

        q_left.col(0)(0) = quat_0.w();
        q_left.col(0)(1) = quat_0.x();
        q_left.col(0)(2) = quat_0.y();
        q_left.col(0)(3) = quat_0.z();

        q_left.col(1)(0) = quat_1.w();
        q_left.col(1)(1) = quat_1.x();
        q_left.col(1)(2) = quat_1.y();
        q_left.col(1)(3) = quat_1.z();

        MatrixXd q_right(4, 1);

        q_right.col(0)(0) = quat_0.w();
        q_right.col(0)(1) = quat_0.x();
        q_right.col(0)(2) = quat_0.y();
        q_right.col(0)(3) = quat_0.z();

        auto diff = utils::diff_quaternion(q_left, q_right);

        std::cerr << "diff_quaternion produced output with " << diff.rows() << " rows (should be 3)" << std::endl;
        if (diff.rows() != 3)
            return EXIT_FAILURE;

        std::cerr << "diff_quaternion produced output with " << diff.cols() << " cols (should be 2)" << std::endl;
        if (diff.cols() != 2)
            return EXIT_FAILURE;

        std::cerr << "The first column is " << diff.col(0).transpose() << " (should be " << rot_diff_0.transpose() << ")" << std::endl;
        if (!(diff.col(0).isApprox(rot_diff_0, 1e-5)))
            return EXIT_FAILURE;

        std::cerr << "The second column is " << diff.col(1).transpose() << " (should be " << rot_diff_1.transpose() << ")" << std::endl;
        if (!(diff.col(1).isApprox(rot_diff_1, 1e-5)))
            return EXIT_FAILURE;
    }

    std::cout << "Test mean_quaternion...\n" << std::endl;
    {
        Quaterniond q_identity = Quaterniond::Identity();

        MatrixXd q(4, 2);

        q.col(0)(0) = quat_1.w();
        q.col(0)(1) = quat_1.x();
        q.col(0)(2) = quat_1.y();
        q.col(0)(3) = quat_1.z();

        q.col(1)(0) = quat_1.w();
        q.col(1)(1) = quat_1.x();
        q.col(1)(2) = quat_1.y();
        q.col(1)(3) = quat_1.z();

        MatrixXd w(2, 1);
        w << 0.5, -0.5;

        auto mean = utils::mean_quaternion(w, q);

        std::cerr << "mean_quaternion produced output with " << mean.rows() << " rows (should be 4)" << std::endl;
        if (mean.rows() != 4)
            return EXIT_FAILURE;

        std::cerr << "mean_quaternion produced output with " << mean.cols() << " cols (should be 1)" << std::endl;
        if (mean.cols() != 1)
            return EXIT_FAILURE;

        std::cerr << "The first column is " << mean.col(0).transpose() << " (should be " << q_identity.w() << " " << q_identity.vec() << ")" << std::endl;
        if (!(Quaterniond(mean.col(0)(0), mean.col(0)(1), mean.col(0)(2), mean.col(0)(3)).isApprox(q_identity, 1e-5)))
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
