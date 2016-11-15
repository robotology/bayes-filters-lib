#ifndef SIRPARTICLEFILTER_H
#define SIRPARTICLEFILTER_H

#include "FilteringAlgorithm.h"
#include "ParticleFilteringFunction.h"

#include <vector>
#include <random>

#include <Eigen/Dense>


class SIRParticleFilter: public FilteringAlgorithm {
protected:

    ParticleFilteringFunction       * _pf_f;

    Eigen::MatrixXf                   _object;
    Eigen::MatrixXf                   _measurement;
    
    Eigen::MatrixXf                   _init_particle;
    Eigen::VectorXf                   _init_weight;

    std::vector<Eigen::MatrixXf>      _result_particle;
    std::vector<Eigen::VectorXf>      _result_weight;

    std::mt19937_64                 * generator;
    std::normal_distribution<float> * distribution_obj;
    std::function<float (float)>      gaussian_random;

    void Snapshot();

public:
    
    SIRParticleFilter();
    
    virtual ~SIRParticleFilter();

    virtual bool Configure();

    virtual void runFilter();

    virtual void getResult();

};

#endif /* SIRPARTICLEFILTER_H */