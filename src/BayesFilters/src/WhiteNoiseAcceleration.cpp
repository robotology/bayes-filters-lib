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


WhiteNoiseAcceleration::WhiteNoiseAcceleration(double T, double tilde_q) noexcept :
    WhiteNoiseAcceleration(T, tilde_q, 1)
{ }


WhiteNoiseAcceleration::WhiteNoiseAcceleration() noexcept :
    WhiteNoiseAcceleration(1.0, 1.0, 1)
{ }


WhiteNoiseAcceleration::~WhiteNoiseAcceleration() noexcept
{ }


WhiteNoiseAcceleration::WhiteNoiseAcceleration(const WhiteNoiseAcceleration& wna) :
    generator_(wna.generator_),
    distribution_(wna.distribution_),
    T_(wna.T_),
    F_(wna.F_),
    Q_(wna.Q_),
    tilde_q_(wna.tilde_q_),
    sqrt_Q_(wna.sqrt_Q_),
    gauss_rnd_sample_(wna.gauss_rnd_sample_)
{ }


WhiteNoiseAcceleration::WhiteNoiseAcceleration(WhiteNoiseAcceleration&& wna) noexcept :
    generator_(std::move(wna.generator_)),
    distribution_(std::move(wna.distribution_)),
    T_(wna.T_),
    F_(std::move(wna.F_)),
    Q_(std::move(wna.Q_)),
    tilde_q_(wna.tilde_q_),
    sqrt_Q_(std::move(wna.sqrt_Q_)),
    gauss_rnd_sample_(std::move(wna.gauss_rnd_sample_))
{
    wna.T_       = 0.0;
    wna.tilde_q_ = 0.0;
}


WhiteNoiseAcceleration& WhiteNoiseAcceleration::operator=(const WhiteNoiseAcceleration& wna)
{
    WhiteNoiseAcceleration tmp(wna);
    *this = std::move(tmp);

    return *this;
}


WhiteNoiseAcceleration& WhiteNoiseAcceleration::operator=(WhiteNoiseAcceleration&& wna) noexcept
{
    if (this == &wna)
        return *this;

    T_       = wna.T_;
    F_       = std::move(wna.F_);
    Q_       = std::move(wna.Q_);
    tilde_q_ = wna.tilde_q_;

    sqrt_Q_           = std::move(wna.sqrt_Q_);
    generator_        = std::move(wna.generator_);
    distribution_     = std::move(wna.distribution_);
    gauss_rnd_sample_ = std::move(wna.gauss_rnd_sample_);

    wna.T_       = 0.0;
    wna.tilde_q_ = 0.0;

    return *this;
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


std::pair<std::size_t, std::size_t> WhiteNoiseAcceleration::getOutputSize() const
{
    return std::make_pair(4, 0);
}
