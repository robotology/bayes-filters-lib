#ifndef UNSCENTEDKALMANFILTER_H
#define UNSCENTEDKALMANFILTER_H

#include <FilteringAlgorithm/FilteringAlgorithm.h>
#include <FilteringFunction_old/UnscentedKalmanFilteringFunction.h>

#include <Eigen/Dense>


class UnscentedKalmanFilter: public FilteringAlgorithm {
private:

    UnscentedKalmanFilteringFunction * _ukf_f;
    
public:

    UnscentedKalmanFilter();

    virtual ~UnscentedKalmanFilter();

};

#endif /* UNSCENTEDKALMANFILTER_H */
