#include "ASIRParticleFilter.h"

/**
 * ASIRParticleFilter implementation
 */


ASIRParticleFilter::ASIRParticleFilter() {};

ASIRParticleFilter::ASIRParticleFilter(AuxiliaryParticleFilteringFunction apf_f) : _apf_f(apf_f) {};