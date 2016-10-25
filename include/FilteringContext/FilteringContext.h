#ifndef FILTERINGCONTEXT_H
#define FILTERINGCONTEXT_H

#include "FilteringAlgorithm.h"


class FilteringContext {
private:

    FilteringAlgorithm * _filter;

public:

    FilteringContext(FilteringAlgorithm * filter);
    
    void run();

    void saveResult();

    void setParameter();
    
};

#endif /* FILTERINGCONTEXT_H */