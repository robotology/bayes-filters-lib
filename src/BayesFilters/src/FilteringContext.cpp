/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/FilteringContext.h>

using namespace bfl;


FilteringContext::FilteringContext(std::shared_ptr<FilteringAlgorithm> filter) : filter_(filter) { };


FilteringContext::~FilteringContext() { };


void FilteringContext::run() { }


void FilteringContext::saveResult() { }


void FilteringContext::setParameter() { };
