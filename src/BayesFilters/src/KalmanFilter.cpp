#include <BayesFilters/KalmanFilter.h>

using namespace bfl;


KalmanFilter::KalmanFilter() { }


KalmanFilter::~KalmanFilter() noexcept { }


bool KalmanFilter::initialization()
{
    return false;
}


void KalmanFilter::filteringStep() { }


void KalmanFilter::getResult() { }
