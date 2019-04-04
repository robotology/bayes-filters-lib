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
