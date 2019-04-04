/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef FILTERINGCONTEXT_H
#define FILTERINGCONTEXT_H

#include <BayesFilters/FilteringAlgorithm.h>

#include <memory>

namespace bfl {
    class FilteringContext;
}


class bfl::FilteringContext
{
public:
    FilteringContext(std::shared_ptr<FilteringAlgorithm> filter);

    virtual ~FilteringContext();

    void run();

    void saveResult();

    void setParameter();

private:
    std::shared_ptr<FilteringAlgorithm> filter_;
};

#endif /* FILTERINGCONTEXT_H */
