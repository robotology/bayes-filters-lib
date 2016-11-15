#include "UnscentedKalmanFilter.h"

/**
 * UnscentedKalmanFilter implementation
 */


UnscentedKalmanFilter::UnscentedKalmanFilter()
{
    _ukf_f = nullptr;
}


UnscentedKalmanFilter::~UnscentedKalmanFilter() {};