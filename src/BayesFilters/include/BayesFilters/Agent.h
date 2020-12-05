/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef AGENT_H
#define AGENT_H

#include <BayesFilters/Data.h>

#include <memory>
#include <string>

#include <Eigen/Dense>

namespace bfl {
    class Agent;
}


class bfl::Agent
{
public:
    virtual ~Agent() noexcept = default;

    virtual bool bufferData() = 0;

    virtual Data getData() const = 0;

    virtual bool setProperty(const std::string& property);
};

#endif /* AGENT_H */
