#ifndef FILTERINGALGORITHM_H
#define FILTERINGALGORITHM_H

class FilteringAlgorithm {
public:
    virtual ~FilteringAlgorithm() noexcept { };

    virtual void runFilter() = 0;

    virtual void getResult() = 0;

};

#endif /* FILTERINGALGORITHM_H */
