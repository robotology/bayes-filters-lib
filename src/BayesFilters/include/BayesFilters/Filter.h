/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef FILTER_H
#define FILTER_H

namespace bfl {
    class Filter;
}


class bfl::Filter
{
public:
    virtual bool boot() = 0;

    virtual void run() = 0;

    virtual bool wait() = 0;

    virtual void reset() = 0;

    virtual void reboot() = 0;

    virtual bool teardown() = 0;

    virtual unsigned int step_number() = 0;

    virtual bool is_running() = 0;
};

#endif /* FILTER_H */
