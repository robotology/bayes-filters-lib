#include "FilteringContext.h"

/**
 * FilteringContext implementation
 */


FilteringContext::FilteringContext(FilteringAlgorithm * filter) : _filter(filter) {};


void FilteringContext::run()
{
    _filter->runFilter();
};


void FilteringContext::saveResult()
{
    _filter->getResult();
};


void FilteringContext::setParameter() {};
