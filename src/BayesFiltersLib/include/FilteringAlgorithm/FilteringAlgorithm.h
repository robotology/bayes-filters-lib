#ifndef FILTERINGALGORITHM_H
#define FILTERINGALGORITHM_H

class FilteringAlgorithm {
public: 

    FilteringAlgorithm() {};

    virtual ~FilteringAlgorithm() {};

    virtual bool Configure() = 0;
    virtual void runFilter() = 0;
    virtual void getResult() = 0;

};

#endif /* FILTERINGALGORITHM_H */