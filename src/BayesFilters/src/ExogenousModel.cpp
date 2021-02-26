/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/ExogenousModel.h>

using namespace bfl;
using namespace Eigen;


bool ExogenousModel::skip(const std::string& what_step, const bool status)
{
    if (what_step == "exogenous")
        skip_ = status;
    else
        return false;

    return true;
}


bool ExogenousModel::is_skipping()
{
    return skip_;
}
