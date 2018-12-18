#ifndef GAUSSIANINITIALIZATION_H
#define GAUSSIANINITIALIZATION_H

#include <BayesFilters/Gaussian.h>

#include <Eigen/Dense>

namespace bfl {
    class GaussianInitialization;
}


class bfl::GaussianInitialization
{
public:
    virtual ~GaussianInitialization() noexcept { };

    virtual void initialize(Gaussian& state) = 0;
};

#endif /* PARTICLESETINITIALIZATION_H */
