/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef DIRECTIONALSTATISTICS_H
#define DIRECTIONALSTATISTICS_H

#include <Eigen/Dense>


namespace bfl
{
namespace directional_statistics
{

    Eigen::MatrixXd directional_add(const Eigen::Ref<const Eigen::MatrixXd>& a, const Eigen::Ref<const Eigen::VectorXd>& b);

    Eigen::MatrixXd directional_sub(const Eigen::Ref<const Eigen::MatrixXd>& a, const Eigen::Ref<const Eigen::VectorXd>& b);

    Eigen::VectorXd directional_mean(const Eigen::Ref<const Eigen::MatrixXd>& a, const Eigen::Ref<const Eigen::VectorXd>& w);

}
}

#endif /* DIRECTIONALSTATISTICS_H */
