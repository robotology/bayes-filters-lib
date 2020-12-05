/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef WHITENOISEACCELERATION_H
#define WHITENOISEACCELERATION_H

#include <BayesFilters/LinearStateModel.h>

#include <functional>
#include <random>

namespace bfl {
    class WhiteNoiseAcceleration;
}


class bfl::WhiteNoiseAcceleration : public LinearStateModel
{
public:
    WhiteNoiseAcceleration(double T, double tilde_q, unsigned int seed) noexcept;

    WhiteNoiseAcceleration(double T, double tilde_q) noexcept;

    WhiteNoiseAcceleration() noexcept;

    WhiteNoiseAcceleration(const WhiteNoiseAcceleration& wna);

    WhiteNoiseAcceleration(WhiteNoiseAcceleration&& wna) noexcept;

    virtual ~WhiteNoiseAcceleration() noexcept = default;

    WhiteNoiseAcceleration& operator=(const WhiteNoiseAcceleration& wna);

    WhiteNoiseAcceleration& operator=(WhiteNoiseAcceleration&& wna) noexcept;

    Eigen::MatrixXd getNoiseSample(const std::size_t num) override;

    Eigen::MatrixXd getNoiseCovarianceMatrix() override;

    Eigen::MatrixXd getStateTransitionMatrix() override;

    Eigen::VectorXd getTransitionProbability(const Eigen::Ref<const Eigen::MatrixXd>& prev_states, const Eigen::Ref<const Eigen::MatrixXd>& cur_states) override;

    bool setProperty(const std::string& property) override { return false; };

    std::pair<std::size_t, std::size_t> getOutputSize() const override;

private:
    std::mt19937_64 generator_;

    std::normal_distribution<double> distribution_;

protected:
    /**
     * Sampling interval in [time].
     */
    double T_;

    /**
     * State transition matrix.
     */
    Eigen::Matrix4d F_;

    /**
     * Convariance matrix of the additive white noise of the state model.
     */
    Eigen::Matrix4d Q_;

    /**
     * Power spectral density [length]^2/[time]^3.
     */
    double tilde_q_;

    /**
     * Square root matrix of R_.
     */
    Eigen::Matrix4d sqrt_Q_;

    /**
     * Random number generator function from a Normal distribution.
     * A call to `gauss_rnd_sample_()` returns a double-precision floating point random number.
     */
    std::function<double()> gauss_rnd_sample_;
};

#endif /* WHITENOISEACCELERATION_H */
