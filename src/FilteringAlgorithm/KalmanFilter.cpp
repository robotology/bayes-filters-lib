#include "KalmanFilter.h"

/**
 * KalmanFilter implementation
 */


KalmanFilter::KalmanFilter() {};

KalmanFilter::KalmanFilter(KalmanFilteringFunction kf_f) : _kf_f(kf_f) {};