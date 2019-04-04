/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/UnscentedKalmanFilter.h>

using namespace bfl;


UnscentedKalmanFilter::UnscentedKalmanFilter() { }


UnscentedKalmanFilter::~UnscentedKalmanFilter() noexcept { }


bool UnscentedKalmanFilter::initialization()
{
    return false;
}


void UnscentedKalmanFilter::filteringStep() { }
