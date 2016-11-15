#ifndef ASIRPARTICLEFILTER_H
#define ASIRPARTICLEFILTER_H

#include "FilteringAlgorithm.h"
#include "AuxiliaryParticleFilteringFunction.h"

#include <Eigen/Dense>

class ASIRParticleFilter: public FilteringAlgorithm {
private:

    AuxiliaryParticleFilteringFunction * _apf_f;

public:
    
    ASIRParticleFilter();

    virtual ~ASIRParticleFilter();
};

#endif /* ASIRPARTICLEFILTER_H */