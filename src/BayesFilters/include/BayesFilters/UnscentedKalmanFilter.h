#ifndef UNSCENTEDKALMANFILTER_H
#define UNSCENTEDKALMANFILTER_H

#include <BayesFilters/FilteringAlgorithm.h>

namespace bfl {
    class UnscentedKalmanFilter;
}


class bfl::UnscentedKalmanFilter: public FilteringAlgorithm
{
public:
    UnscentedKalmanFilter();

    virtual ~UnscentedKalmanFilter() noexcept;

    bool initialization() override;

    void filteringStep() override;

    void getResult() override;
};

#endif /* UNSCENTEDKALMANFILTER_H */
