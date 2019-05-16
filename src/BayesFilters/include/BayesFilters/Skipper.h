/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef SKIPPER_H
#define SKIPPER_H

#include <string>

namespace bfl {
    class Skipper;
}


class bfl::Skipper
{
public:
    virtual bool skip(const std::string& what_step, const bool status) = 0;
};

#endif /* SKIPPER_H */
