#ifndef PARTICLEFILTERINGFUNCTION_H
#define PARTICLEFILTERINGFUNCTION_H

#include "BayesianFilteringFunction.h"

#include <random>

class ParticleFilteringFunction: public BayesianFilteringFunction {
private:

    double _state_cov;
    Eigen::Matrix2f _obs_cov;

    std::mt19937_64                  * generator;
    std::normal_distribution<float>  * distribution_pos;
    std::normal_distribution<float>  * distribution_vel;
    std::function<float (float)>     gaussian_random_pos;
    std::function<float (float)>     gaussian_random_vel;

public:

    ParticleFilteringFunction();

    ~ParticleFilteringFunction();

    virtual void StateModel(const Eigen::Ref<const Eigen::VectorXf> & prev_state, Eigen::Ref<Eigen::VectorXf> prop_state);

    virtual Eigen::Ref<Eigen::VectorXf> ObservationModel(const Eigen::Ref<const Eigen::VectorXf> & pred_state);

    virtual void Prediction(const Eigen::Ref<const Eigen::VectorXf> & prev_state, Eigen::Ref<Eigen::VectorXf> pred_state);

    virtual void Correction(const Eigen::Ref<const Eigen::VectorXf> & pred_state, const Eigen::Ref<const Eigen::VectorXf> & measurements, Eigen::Ref<Eigen::VectorXf> cor_state);

    virtual void Resampling(const Eigen::Ref<const Eigen::MatrixXf> & pred_particles, const Eigen::Ref<const Eigen::VectorXf> & cor_weights, Eigen::Ref<Eigen::MatrixXf> res_particles, Eigen::Ref<Eigen::VectorXf> res_weights, Eigen::Ref<Eigen::VectorXf> res_parents);

    virtual Eigen::Ref<Eigen::VectorXf> Normalize(const Eigen::Ref<const Eigen::VectorXf> cor_weights);

    virtual float Neff(const Eigen::Ref<const Eigen::VectorXf> & cor_weights);

};

#endif /* PARTICLEFILTERINGFUNCTION_H */