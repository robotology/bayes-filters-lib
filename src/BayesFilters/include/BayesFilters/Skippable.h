/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef SKIPPABLE_H
#define SKIPPABLE_H

#include <BayesFilters/Skipper.h>

namespace bfl {
    class Skippable;
}


class bfl::Skippable : public Skipper
{
public:
    virtual bool is_skipping() = 0;
};

#endif /* SKIPPABLE_H */
