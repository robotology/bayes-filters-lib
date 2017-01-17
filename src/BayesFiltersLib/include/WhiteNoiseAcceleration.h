#ifndef WHITENOISEACCELERATION_H
#define WHITENOISEACCELERATION_H

#include <functional>
#include <random>

#include "StateModel.h"


class WhiteNoiseAcceleration : public StateModel {
public:
    /* WNA complete constructor */
    WhiteNoiseAcceleration(float T, float tilde_q, unsigned int seed) noexcept;

    /* WNA constructor, no rnd seed */
    WhiteNoiseAcceleration(float T, float tilde_q) noexcept;

    /* Default constructor */
    WhiteNoiseAcceleration() noexcept;

    /* Destructor */
    ~WhiteNoiseAcceleration() noexcept override;

    /* Copy constructor */
    WhiteNoiseAcceleration(const WhiteNoiseAcceleration& wna);

    /* Move constructor */
    WhiteNoiseAcceleration(WhiteNoiseAcceleration&& wna) noexcept;

    /* Copy assignment operator */
    WhiteNoiseAcceleration& operator=(const WhiteNoiseAcceleration& wna);

    /* Move assignment operator */
    WhiteNoiseAcceleration& operator=(WhiteNoiseAcceleration&& wna) noexcept;

    void propagate(const Eigen::Ref<const Eigen::VectorXf> & cur_state, Eigen::Ref<Eigen::VectorXf> prop_state) override;

    void noiseSample(Eigen::Ref<Eigen::VectorXf> sample) override;

    void motion(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> next_state) override;

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
