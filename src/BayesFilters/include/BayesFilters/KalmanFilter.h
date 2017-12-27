#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include "FilteringAlgorithm.h"

namespace bfl {
    class KalmanFilter;
}


class bfl::KalmanFilter: public FilteringAlgorithm {
public:
    KalmanFilter();

    virtual ~KalmanFilter() noexcept;

    void initialization() override;
    
    void filteringStep() override;

    void getResult() override;
};

#endif /* KALMANFILTER_H */
