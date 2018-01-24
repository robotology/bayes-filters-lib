#ifndef UNSCENTEDKALMANFILTER_H
#define UNSCENTEDKALMANFILTER_H

#include "FilteringAlgorithm.h"

namespace bfl {
    class UnscentedKalmanFilter;
}


class bfl::UnscentedKalmanFilter: public FilteringAlgorithm
{
public:
    UnscentedKalmanFilter();

    virtual ~UnscentedKalmanFilter() noexcept;

    void initialization() override;

    void filteringStep() override;

    void getResult() override;
};

#endif /* UNSCENTEDKALMANFILTER_H */
