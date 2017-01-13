#ifndef UNSCENTEDKALMANFILTER_H
#define UNSCENTEDKALMANFILTER_H

#include <Eigen/Dense>

#include <FilteringAlgorithm/FilteringAlgorithm.h>


class UnscentedKalmanFilter: public FilteringAlgorithm {
public:
    UnscentedKalmanFilter();

    virtual ~UnscentedKalmanFilter();

    virtual void runFilter() = 0;

    virtual void getResult() = 0;
};

#endif /* UNSCENTEDKALMANFILTER_H */
