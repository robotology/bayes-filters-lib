#ifndef PARTICLESETINITIALIZATION_H
#define PARTICLESETINITIALIZATION_H

#include <BayesFilters/ParticleSet.h>

#include <Eigen/Dense>

namespace bfl {
    class ParticleSetInitialization;
}


class bfl::ParticleSetInitialization
{
public:
    virtual ~ParticleSetInitialization() noexcept { };

    virtual bool initialize(ParticleSet& particles) = 0;
};

#endif /* PARTICLESETINITIALIZATION_H */
