/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef UNSCENTEDKALMANFILTER_H
#define UNSCENTEDKALMANFILTER_H

#include <BayesFilters/FilteringAlgorithm.h>

namespace bfl {
    class UnscentedKalmanFilter;
}


class bfl::UnscentedKalmanFilter: public FilteringAlgorithm
{
public:
    UnscentedKalmanFilter();

    virtual ~UnscentedKalmanFilter() noexcept;

    bool initialization() override;

    void filteringStep() override;
};

#endif /* UNSCENTEDKALMANFILTER_H */
