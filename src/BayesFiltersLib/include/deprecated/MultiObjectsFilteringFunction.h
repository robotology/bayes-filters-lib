#ifndef MULTIOBJECTSFILTERINGFUNCTION_H
#define MULTIOBJECTSFILTERINGFUNCTION_H

#include <FilteringFunction_old/BayesianFilteringFunction.h>


class MultiObjectsFilteringFunction: public BayesianFilteringFunction {
public: 
    
    virtual void ClutterModel()     = 0;
    virtual void ObjectExtraction() = 0;
};

#endif /* MULTIOBJECTSFILTERINGFUNCTION_H */
