#include "BayesFiltersLib/FilteringContext.h"


namespace bfl
{

FilteringContext::FilteringContext(std::shared_ptr<FilteringAlgorithm> filter) : filter_(filter) { };


FilteringContext::~FilteringContext() { };


void FilteringContext::run()
{
    filter_->runFilter();
}


void FilteringContext::saveResult()
{
    filter_->getResult();
}


void FilteringContext::setParameter() { };

} // namespace bfl
