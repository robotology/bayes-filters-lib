#include <BayesFilters/UnscentedKalmanFilter.h>

using namespace bfl;


UnscentedKalmanFilter::UnscentedKalmanFilter() { }


UnscentedKalmanFilter::~UnscentedKalmanFilter() noexcept { }


bool UnscentedKalmanFilter::initialization()
{
    return false;
}


void UnscentedKalmanFilter::filteringStep() { }


void UnscentedKalmanFilter::getResult() { }
