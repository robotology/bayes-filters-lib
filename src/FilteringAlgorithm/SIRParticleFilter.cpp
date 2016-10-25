#include "SIRParticleFilter.h"

#include <fstream>
#include <iostream>
#include <random>

using namespace Eigen;

/**
 * SIRParticleFilter implementation
 */

Vector2f LIN(const Ref<const Vector4f> state);


SIRParticleFilter::SIRParticleFilter()
{
    generator         = new std::mt19937_64(1);
    distribution_obj  = new std::normal_distribution<float>(0.0,   5.0);
    gaussian_random   = [&] (int) { return (*distribution_obj)(*generator); };
};


SIRParticleFilter::SIRParticleFilter(ParticleFilteringFunction pf_f) : _pf_f(pf_f)
{
    SIRParticleFilter();
};


SIRParticleFilter::~SIRParticleFilter()
{
    delete generator;
    delete distribution_obj;
}

void SIRParticleFilter::runFilter()
{
    /* GENERATE MEASUREMENTS */
    _measurement.resize(2, 100);
    _object.resize(4, 100);
    
    _object.col(0) << 0, 10, 0, 10;
    _measurement.col(0) << LIN(_object.col(0));

    Matrix4f A;
    A << 1, 5, 0, 0,
         0, 1, 0, 0,
         0, 0, 1, 5,
         0, 0, 0, 1;
    for (int k = 1; k < 100; ++k)
    {
        _object.col(k) = A * _object.col(k-1) + VectorXf::NullaryExpr(4, gaussian_random);
        _measurement.col(k) << LIN(_object.col(k));
    }

    /* INITIALIZATION */
    int num_particle = 10000;
    int surv_x = 5000;
    int surv_y = 5000;

    _init_weight.resize(num_particle, 1);
    _init_weight.setConstant(1.0/num_particle);

    _init_particle.resize(4, num_particle);
    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 100; ++j)
        {
            _init_particle.col(i*100 + j) << (surv_x / 100) * i,
                                                              0,
                                             (surv_y / 100) * j,
                                                              0;
        }
    }

    _result_particle.resize(100);
    _result_weight.resize(100);

    /* FILTERING */
    for (int k = 0; k < 100; ++k)
    {
        MatrixXf temp_particle(4, num_particle);
        VectorXf temp_weight(num_particle, 1);
        VectorXf temp_parent(num_particle, 1);

//        Snapshot();

        for (int i = 0; i < num_particle; ++i)
        {
            _pf_f.Prediction(_init_particle.col(i), _init_particle.col(i));
        }
//        Snapshot();
        for (int i = 0; i < num_particle; ++i)
        {
            _pf_f.Correction(_init_particle.col(i), _measurement.col(k), _init_weight.row(i));
        }

        _init_weight = _pf_f.Normalize(_init_weight);

//        Snapshot();

        _result_particle[k] = _init_particle;
        _result_weight  [k] = _init_weight;

        if (_pf_f.Neff(_init_weight) < num_particle/3)
        {
            _pf_f.Resampling(_init_particle, _init_weight, temp_particle, temp_weight, temp_parent);

            _init_particle = temp_particle;
            _init_weight = temp_weight;
        }

    }
};

void SIRParticleFilter::Snapshot()
{
    std::ofstream result_file_object;
    std::ofstream result_file_measurement;
    std::ofstream result_file_particle;
    std::ofstream result_file_weight;

    result_file_object.open     ("../../dbg/result_object.txt"     , std::ios_base::out | std::ios_base::trunc);
    result_file_measurement.open("../../dbg/result_measurement.txt", std::ios_base::out | std::ios_base::trunc);
    result_file_particle.open   ("../../dbg/result_particle.txt"   , std::ios_base::out | std::ios_base::trunc);
    result_file_weight.open     ("../../dbg/result_weight.txt"     , std::ios_base::out | std::ios_base::trunc);

    result_file_object      << _object;
    result_file_measurement << _measurement;
    result_file_particle    << _init_particle;
    result_file_weight      << _init_weight;

    result_file_object.close();
    result_file_measurement.close();
    result_file_particle.close();
    result_file_weight.close();
}


void SIRParticleFilter::getResult()
{
    std::ofstream result_file_object;
    std::ofstream result_file_measurement;
    std::ofstream result_file_particle;
    std::ofstream result_file_weight;

    result_file_object.open     ("../../dbg/result_object.txt");
    result_file_measurement.open("../../dbg/result_measurement.txt");
    result_file_particle.open   ("../../dbg/result_particle.txt");
    result_file_weight.open     ("../../dbg/result_weight.txt");

    result_file_object << _object;
    result_file_measurement << _measurement;
    for (int k = 0; k < 100; ++k)
    {
        result_file_particle << _result_particle[k] << std::endl << std::endl;

        result_file_weight << _result_weight[k] << std::endl << std::endl;
    }

    result_file_object.close();
    result_file_measurement.close();
    result_file_particle.close();
    result_file_weight.close();
};


std::mt19937_64 generator_LIN(1);
std::normal_distribution<float> distribution_LIN(0.0, 100.0);

Vector2f LIN(const Ref<const Vector4f> state)
{
    auto gaussian_random = [&] (int) { return distribution_LIN(generator_LIN); };
    
    return Vector2f(state(0) + VectorXf::NullaryExpr(1, gaussian_random)(0),
                    state(2) + VectorXf::NullaryExpr(1, gaussian_random)(0));
}
