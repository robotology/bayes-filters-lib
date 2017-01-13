#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <Eigen/Dense>

#include <FilteringAlgorithm/FilteringAlgorithm.h>

class KalmanFilter: public FilteringAlgorithm {
public:
    KalmanFilter();

    virtual ~KalmanFilter();

    virtual void runFilter() = 0;

    virtual void getResult() = 0;
};

#endif /* KALMANFILTER_H */
