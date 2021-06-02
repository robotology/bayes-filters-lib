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


struct WhiteNoiseAcceleration::ImplData
{
    ImplData
    (
        const Dim dim,
        const double sampling_interval,
        const double tilde_q,
        const unsigned int seed
    ) :
        T_(sampling_interval),
        tilde_q_(tilde_q),
        seed_(seed),
        generator_(std::mt19937_64(seed_)),
        distribution_(std::normal_distribution<double>(0.0, 1.0)),
        gauss_rnd_sample_([&] { return (distribution_) (generator_); })
    {
        double q11 = 1.0 / 3.0 * std::pow(T_, 3.0);
        double q2 = 1.0 / 2.0 * std::pow(T_, 2.0);

        Matrix2d F;
        F << 1.0, T_,
             0.0, 1.0;

        Matrix2d Q;
        Q << q11, q2,
              q2, T_;

        switch (dim)
        {
            case Dim::OneD:
            {
                F_ = F;

                Q_ = Q;

                break;
            }

            case Dim::TwoD:
            {
                F_ = MatrixXd(4, 4);
                F_ << F, Matrix2d::Zero(),
                      Matrix2d::Zero(), F;

                Q_ = MatrixXd(4, 4);
                Q_ << Q, Matrix2d::Zero(),
                      Matrix2d::Zero(), Q;

                break;
            }

            case Dim::ThreeD:
            {
                F_ = MatrixXd(6, 6);
                F_ << F, Matrix2d::Zero(), Matrix2d::Zero(),
                      Matrix2d::Zero(), F, Matrix2d::Zero(),
                      Matrix2d::Zero(), Matrix2d::Zero(), F;

                Q_ = MatrixXd(6, 6);
                Q_ << Q, Matrix2d::Zero(), Matrix2d::Zero(),
                      Matrix2d::Zero(), Q, Matrix2d::Zero(),
                      Matrix2d::Zero(), Matrix2d::Zero(), Q;

                break;
            }
        }

        Q_ *= tilde_q_;

        LDLT<MatrixXd> chol_ldlt(Q_);
        sqrt_Q_ = (chol_ldlt.transpositionsP() * MatrixXd::Identity(Q_.rows(), Q_.cols())).transpose() * chol_ldlt.matrixL() * chol_ldlt.vectorD().real().cwiseSqrt().asDiagonal();
    }

    /**
     * Sampling interval in [time].
     */
    double T_;

    /**
     * Power spectral density [length]^2/[time]^3.
     */
    double tilde_q_;

    /**
     * State transition matrix.
     */
    Eigen::MatrixXd F_;

    /**
     * Convariance matrix of the additive white noise of the state model.
     */
    Eigen::MatrixXd Q_;

    /**
     * Square root matrix of R_.
     */
    Eigen::MatrixXd sqrt_Q_;

    /**
     * Seed of the random number generator.
     */
    unsigned int seed_;

    /**
     * Random number generator.
     */
    std::mt19937_64 generator_;

    /**
     * Normal distribution for random number generation functions.
     */
    std::normal_distribution<double> distribution_;

    /**
     * Random number generator function from a Normal distribution.
     * A call to `gauss_rnd_sample_()` returns a double-precision floating point random number.
     */
    std::function<double()> gauss_rnd_sample_;
};


WhiteNoiseAcceleration::WhiteNoiseAcceleration
(
    const Dim dim,
    const double sampling_interval,
    const double tilde_q
) noexcept :
    WhiteNoiseAcceleration(dim, sampling_interval, tilde_q, 1)
{ }


WhiteNoiseAcceleration::WhiteNoiseAcceleration
(
    const Dim dim,
    const double sampling_interval,
    const double tilde_q,
    const unsigned int seed
) noexcept :
    pimpl_(utils::make_unique<ImplData>(dim, sampling_interval, tilde_q, seed))
{ }


WhiteNoiseAcceleration::WhiteNoiseAcceleration(WhiteNoiseAcceleration&& state_model) noexcept = default;


WhiteNoiseAcceleration& WhiteNoiseAcceleration::operator=(WhiteNoiseAcceleration&& state_model) noexcept = default;


WhiteNoiseAcceleration::~WhiteNoiseAcceleration() noexcept = default;


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
        *(rand_vectors.data() + i) = pimpl_->gauss_rnd_sample_();

    return pimpl_->sqrt_Q_ * rand_vectors;
}


MatrixXd WhiteNoiseAcceleration::getNoiseCovarianceMatrix()
{
    return pimpl_->Q_;
}


MatrixXd WhiteNoiseAcceleration::getStateTransitionMatrix()
{
    return pimpl_->F_;
}


VectorXd WhiteNoiseAcceleration::getTransitionProbability(const Ref<const MatrixXd>& prev_states, const Ref<const MatrixXd>& cur_states)
{
    return utils::multivariate_gaussian_density(prev_states, prev_states.col(0), pimpl_->Q_);
}
