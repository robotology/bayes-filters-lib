#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <BayesFilters/FilteringAlgorithm.h>

namespace bfl {
    class KalmanFilter;
}


class bfl::KalmanFilter: public FilteringAlgorithm {
public:
    KalmanFilter();

    virtual ~KalmanFilter() noexcept;

    bool initialization() override;

    void filteringStep() override;

    void getResult() override;
};

#endif /* KALMANFILTER_H */
