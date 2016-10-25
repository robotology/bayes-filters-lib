#ifndef ASIRPARTICLEFILTER_H
#define ASIRPARTICLEFILTER_H

#include "FilteringAlgorithm.h"
#include "AuxiliaryParticleFilteringFunction.h"

#include <Eigen/Dense>

class ASIRParticleFilter: public FilteringAlgorithm {
private:

    AuxiliaryParticleFilteringFunction _apf_f;

public:
    
    ASIRParticleFilter();
    
    /**
     * @param AuxiliaryParticleFilteringFunction
     */
    ASIRParticleFilter(AuxiliaryParticleFilteringFunction);
};

#endif /* ASIRPARTICLEFILTER_H */