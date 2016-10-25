#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include "FilteringAlgorithm.h"
#include "KalmanFilteringFunction.h"

#include <Eigen/Dense>

class KalmanFilter: public FilteringAlgorithm {
private:

    KalmanFilteringFunction _kf_f;

public:
    
    KalmanFilter();
    
    /**
     * @param KalmanFilteringFunction
     */
    KalmanFilter(KalmanFilteringFunction);
};

#endif /* KALMANFILTER_H */