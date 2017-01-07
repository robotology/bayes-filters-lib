#ifndef UNSCENTEKALMANFILTERINGFUNCTION_H
#define UNSCENTEKALMANFILTERINGFUNCTION_H

#include <FilteringFunction_old/KalmanFilteringFunction.h>


class UnscentedKalmanFilteringFunction: public KalmanFilteringFunction {
public:
    
    virtual void UnscentedTransform();
    
};

#endif /* UNSCENTEKALMANFILTERINGFUNCTION_H */
