#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include "FilteringAlgorithm.h"

namespace bfl
{

class KalmanFilter: public FilteringAlgorithm {
public:
    KalmanFilter();

    ~KalmanFilter() noexcept override;

    void runFilter() override;

    void getResult() override;
};

} // namespace bfl

#endif /* KALMANFILTER_H */
