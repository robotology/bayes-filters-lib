#ifndef UNSCENTEDKALMANFILTER_H
#define UNSCENTEDKALMANFILTER_H

#include "FilteringAlgorithm.h"


namespace bfl
{

class UnscentedKalmanFilter: public FilteringAlgorithm {
public:
    UnscentedKalmanFilter();

    ~UnscentedKalmanFilter() noexcept override;

    void runFilter() override;

    void getResult() override;
};

} // namespace bfl

#endif /* UNSCENTEDKALMANFILTER_H */
