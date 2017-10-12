#ifndef WHITENOISEACCELERATION_H
#define WHITENOISEACCELERATION_H

#include <functional>
#include <random>

#include "StateModel.h"

namespace bfl {
    class WhiteNoiseAcceleration;
}


class bfl::WhiteNoiseAcceleration : public StateModel
{
public:
    WhiteNoiseAcceleration(float T, float tilde_q, unsigned int seed) noexcept;

    WhiteNoiseAcceleration(float T, float tilde_q) noexcept;

    WhiteNoiseAcceleration() noexcept;

    ~WhiteNoiseAcceleration() noexcept override;

    WhiteNoiseAcceleration(const WhiteNoiseAcceleration& wna);

    WhiteNoiseAcceleration(WhiteNoiseAcceleration&& wna) noexcept;

    WhiteNoiseAcceleration& operator=(const WhiteNoiseAcceleration& wna);

    WhiteNoiseAcceleration& operator=(WhiteNoiseAcceleration&& wna) noexcept;

    void propagate(const Eigen::Ref<const Eigen::MatrixXf>& cur_state, Eigen::Ref<Eigen::MatrixXf> prop_state) override;

    void motion(const Eigen::Ref<const Eigen::MatrixXf>& cur_state, Eigen::Ref<Eigen::MatrixXf> prop_state) override;

    Eigen::MatrixXf getNoiseSample(const int num) override;

    Eigen::MatrixXf getNoiseCovariance() override;

    bool setProperty(const std::string& property) override { return false; };

protected:
    float                           T_;                /* Sampling interval */
    Eigen::Matrix4f                 F_;                /* State transition matrix */
    Eigen::Matrix4f                 Q_;                /* Process white noise convariance matrix */
    float                           tilde_q_;          /* Power spectral density [length]^2/[time]^3 */

    Eigen::Matrix4f                 sqrt_Q_;           /* Square root matrix of the process white noise convariance matrix */
    std::mt19937_64                 generator_;
    std::normal_distribution<float> distribution_;
    std::function<float()>          gauss_rnd_sample_; /* Random number generator from a Normal distribution */
};

#endif /* WHITENOISEACCELERATION_H */
