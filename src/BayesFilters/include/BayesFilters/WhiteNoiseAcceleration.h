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
    enum class Dim
    {
        OneD,
        TwoD,
        ThreeD
    };

    WhiteNoiseAcceleration(const Dim dim, const double sampling_interval, const double tilde_q) noexcept;

    WhiteNoiseAcceleration(const Dim dim, const double sampling_interval, const double tilde_q, const unsigned int seed) noexcept;

    WhiteNoiseAcceleration(const WhiteNoiseAcceleration& state_model) noexcept = delete;

    WhiteNoiseAcceleration& operator=(const WhiteNoiseAcceleration& state_model) noexcept = delete;

    WhiteNoiseAcceleration(WhiteNoiseAcceleration&& state_model) noexcept;

    WhiteNoiseAcceleration& operator=(WhiteNoiseAcceleration&& state_model) noexcept;

    virtual ~WhiteNoiseAcceleration() noexcept;

    bool setProperty(const std::string& property) override;

    std::pair<std::size_t, std::size_t> getOutputSize() const override;

    Eigen::MatrixXd getNoiseSample(const std::size_t num) override;

    Eigen::MatrixXd getNoiseCovarianceMatrix() override;

    Eigen::MatrixXd getStateTransitionMatrix() override;

    Eigen::VectorXd getTransitionProbability(const Eigen::Ref<const Eigen::MatrixXd>& prev_states, const Eigen::Ref<const Eigen::MatrixXd>& cur_states) override;


private:
    /**
     * The stored content of an object.
     */
    struct ImplData;

    std::unique_ptr<ImplData> pimpl_;
};

#endif /* WHITENOISEACCELERATION_H */
