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
