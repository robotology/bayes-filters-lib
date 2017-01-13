#ifndef ASIRPARTICLEFILTER_H
#define ASIRPARTICLEFILTER_H

#include <Eigen/Dense>

#include <FilteringAlgorithm/FilteringAlgorithm.h>

class ASIRParticleFilter: public FilteringAlgorithm {
public:
    ASIRParticleFilter();

    virtual ~ASIRParticleFilter();

    void runFilter() override;

    void getResult() override;
};

#endif /* ASIRPARTICLEFILTER_H */
