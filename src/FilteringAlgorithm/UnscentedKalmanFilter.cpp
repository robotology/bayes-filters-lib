#include "UnscentedKalmanFilter.h"

/**
 * UnscentedKalmanFilter implementation
 */


UnscentedKalmanFilter::UnscentedKalmanFilter() {};

UnscentedKalmanFilter::UnscentedKalmanFilter(UnscentedKalmanFilteringFunction ukf_f) : _ukf_f(ukf_f) {};