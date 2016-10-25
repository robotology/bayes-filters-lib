#include "ParticleFilteringFunction.h"

#include <iostream>
#include <random>

using namespace Eigen;

/**
 * ParticleFilteringFunction implementation
 */

std::mt19937_64 generator2;
std::normal_distribution<float> distribution2(0.0, 300.0);
std::normal_distribution<float> distribution3(0.0,  10.0);


ParticleFilteringFunction::ParticleFilteringFunction()
{
    _state_cov = 5;
    _obs_cov << 10*10,     0,
                    0, 10*10;

    generator = new std::mt19937_64(1);
    distribution_pos = new std::normal_distribution<float>(0.0, 300.0);
    distribution_vel = new std::normal_distribution<float>(0.0,  10.0);
    gaussian_random_pos = [&] (int) { return (*distribution_pos)(*generator); };
    gaussian_random_vel = [&] (int) { return (*distribution_vel)(*generator); };
    
}


ParticleFilteringFunction::~ParticleFilteringFunction()
{
    delete generator;
    delete distribution_pos;
    delete distribution_vel;
}


void ParticleFilteringFunction::StateModel(const Ref<const VectorXf> & prev_state, Ref<VectorXf> prop_state)
{
    Matrix4f A;
    A << 1, 1, 0, 0,
         0, 1, 0, 0,
         0, 0, 1, 1,
         0, 0, 0, 1;
    prop_state = A * prev_state;
}


Ref<VectorXf> ParticleFilteringFunction::ObservationModel(const Ref<const VectorXf> & pred_state)
{
    VectorXf m(2);
    m << pred_state(0), pred_state(2);
    return m;
}


void ParticleFilteringFunction::Prediction(const Ref<const VectorXf> & prev_state, Ref<VectorXf> pred_state)
{
    StateModel(prev_state, pred_state);

    pred_state(0) += VectorXf::NullaryExpr(1, gaussian_random_pos)(0);
    pred_state(1) += VectorXf::NullaryExpr(1, gaussian_random_vel)(0);
    pred_state(2) += VectorXf::NullaryExpr(1, gaussian_random_pos)(0);
    pred_state(3) += VectorXf::NullaryExpr(1, gaussian_random_vel)(0);
}


void ParticleFilteringFunction::Correction(const Ref<const VectorXf> & pred_particles, const Ref<const VectorXf> & measurements, Ref<VectorXf> cor_state)
{
    VectorXf innovation(2);
    innovation << ObservationModel(pred_particles);

    /* Simple Linear sensor for position observations */
    innovation(0) -= measurements(0);
    innovation(1) -= measurements(1);

    cor_state = (- 0.5 * static_cast<double>(measurements.rows()) * log(2.0*M_PI) - 0.5 * log(_obs_cov.determinant()) - 0.5 * (innovation.transpose() * _obs_cov.inverse() * innovation).array()).exp();
}


void ParticleFilteringFunction::Resampling(const Ref<const MatrixXf> & pred_particles, const Ref<const VectorXf> & cor_weights,
                                           Ref<MatrixXf> res_particles, Ref<VectorXf> res_weights, Ref<VectorXf> res_parents)
{
    int num_particles = static_cast<int>(cor_weights.rows());
    VectorXf csw(num_particles);

    csw(0) = cor_weights(0);
    for (int i = 1; i < num_particles; ++i)
    {
        csw(i) = csw(i-1) + cor_weights(i);
    }

    std::uniform_real_distribution<float> distribution_res(0.0, 1.0/num_particles);
    auto uniform = [&] (int) {return distribution_res(*generator);};
    float u_1 = VectorXf::NullaryExpr(1, uniform)(0);

    int idx_csw = 0;
    for (int j = 0; j < num_particles; ++j)
    {
        float u_j = u_1 + static_cast<float>((j))/num_particles;
        while (u_j > csw(idx_csw)) { idx_csw += 1; }
        res_particles.col(j) = pred_particles.col(idx_csw);
        res_weights(j)       = 1.0/num_particles;
        res_parents(j)       = idx_csw;
    }

}


Ref<VectorXf> ParticleFilteringFunction::Normalize(const Ref<const VectorXf> cor_weights)
{
    VectorXf cor_weights_normalized = cor_weights / cor_weights.sum();
    return cor_weights_normalized;
}


float ParticleFilteringFunction::Neff(const Ref<const VectorXf> & cor_weights)
{
    return 1.0/cor_weights.array().square().sum();
}
