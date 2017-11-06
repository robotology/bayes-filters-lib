#include "BayesFilters/FilteringContext.h"

using namespace bfl;


FilteringContext::FilteringContext(std::shared_ptr<FilteringAlgorithm> filter) : filter_(filter) { };


FilteringContext::~FilteringContext() { };


void FilteringContext::run() { }


void FilteringContext::saveResult() { }


void FilteringContext::setParameter() { };
