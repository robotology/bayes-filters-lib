#ifndef FILTERINGCONTEXT_H
#define FILTERINGCONTEXT_H

#include <memory>

#include "FilteringAlgorithm.h"


class FilteringContext {
public:
    FilteringContext(std::shared_ptr<FilteringAlgorithm> filter);

    ~FilteringContext();
    
    void run();

    void saveResult();

    void setParameter();

private:
    std::shared_ptr<FilteringAlgorithm> filter_;
};

#endif /* FILTERINGCONTEXT_H */
