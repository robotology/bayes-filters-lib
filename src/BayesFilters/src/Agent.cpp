/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/Agent.h>

#include <iostream>

using namespace bfl;


bool Agent::setProperty(const std::string& property)
{
    static_cast<void>(property);
    return false;
}
