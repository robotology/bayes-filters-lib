/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/WhiteNoiseAcceleration.h>
#include <BayesFilters/utils.h>

#include <cmath>
#include <utility>

#include <Eigen/Cholesky>

using namespace bfl;
using namespace Eigen;


WhiteNoiseAcceleration::WhiteNoiseAcceleration() noexcept :
    WhiteNoiseAcceleration(1.0, 1.0, 1)
{ }


WhiteNoiseAcceleration::WhiteNoiseAcceleration(double T, double tilde_q) noexcept :
    WhiteNoiseAcceleration(T, tilde_q, 1)
{ }


WhiteNoiseAcceleration::WhiteNoiseAcceleration
(
    double T,
    double tilde_q,
    unsigned int seed
) noexcept :
    generator_(std::mt19937_64(seed)),
    distribution_(std::normal_distribution<double>(0.0, 1.0)),
    T_(T),
    tilde_q_(tilde_q),
    gauss_rnd_sample_([&] { return (distribution_)(generator_); })
{
    F_ << 1.0,  T_, 0.0, 0.0,
          0.0, 1.0, 0.0, 0.0,
          0.0, 0.0, 1.0,  T_,
          0.0, 0.0, 0.0, 1.0;

    double q11 = 1.0/3.0 * std::pow(T_, 3.0);
    double q2  = 1.0/2.0 * std::pow(T_, 2.0);
    Q_ << q11,  q2, 0.0, 0.0,
           q2,  T_, 0.0, 0.0,
          0.0, 0.0, q11,  q2,
          0.0, 0.0,  q2,  T_;
    Q_ *= tilde_q;

    LDLT<Matrix4d> chol_ldlt(Q_);
    sqrt_Q_ = (chol_ldlt.transpositionsP() * Matrix4d::Identity()).transpose() * chol_ldlt.matrixL() * chol_ldlt.vectorD().real().cwiseSqrt().asDiagonal();
}


WhiteNoiseAcceleration::WhiteNoiseAcceleration(WhiteNoiseAcceleration&& state_model) noexcept :
    generator_(std::move(state_model.generator_)),
    distribution_(std::move(state_model.distribution_)),
    T_(state_model.T_),
    F_(std::move(state_model.F_)),
    Q_(std::move(state_model.Q_)),
    tilde_q_(state_model.tilde_q_),
    sqrt_Q_(std::move(state_model.sqrt_Q_)),
    gauss_rnd_sample_(std::move(state_model.gauss_rnd_sample_))
{
    state_model.T_       = 0.0;
    state_model.tilde_q_ = 0.0;
}


WhiteNoiseAcceleration& WhiteNoiseAcceleration::operator=(WhiteNoiseAcceleration&& state_model) noexcept
{
    if (this == &state_model)
        return *this;

    T_ = state_model.T_;

    F_ = std::move(state_model.F_);

    Q_ = std::move(state_model.Q_);

    tilde_q_ = state_model.tilde_q_;

    sqrt_Q_ = std::move(state_model.sqrt_Q_);

    generator_ = std::move(state_model.generator_);

    distribution_ = std::move(state_model.distribution_);

    gauss_rnd_sample_ = std::move(state_model.gauss_rnd_sample_);

    state_model.T_ = 0.0;

    state_model.tilde_q_ = 0.0;

    return *this;
}


bool WhiteNoiseAcceleration::setProperty(const std::string& property)
{
    return false;
}


std::pair<std::size_t, std::size_t> WhiteNoiseAcceleration::getOutputSize() const
{
    return std::make_pair(4, 0);
}


MatrixXd WhiteNoiseAcceleration::getNoiseSample(const std::size_t num)
{
    MatrixXd rand_vectors(4, num);
    for (int i = 0; i < rand_vectors.size(); i++)
        *(rand_vectors.data() + i) = gauss_rnd_sample_();

    return sqrt_Q_ * rand_vectors;
}


MatrixXd WhiteNoiseAcceleration::getNoiseCovarianceMatrix()
{
    return Q_;
}


MatrixXd WhiteNoiseAcceleration::getStateTransitionMatrix()
{
    return F_;
}


VectorXd WhiteNoiseAcceleration::getTransitionProbability(const Ref<const MatrixXd>& prev_states, const Ref<const MatrixXd>& cur_states)
{
    return utils::multivariate_gaussian_density(prev_states, prev_states.col(0), Q_);
}
