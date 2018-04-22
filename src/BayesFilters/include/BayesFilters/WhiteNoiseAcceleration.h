#ifndef WHITENOISEACCELERATION_H
#define WHITENOISEACCELERATION_H

#include <BayesFilters/StateModel.h>

#include <functional>
#include <random>

namespace bfl {
    class WhiteNoiseAcceleration;
}


class bfl::WhiteNoiseAcceleration : public StateModel
{
public:
    WhiteNoiseAcceleration(float T, float tilde_q, unsigned int seed) noexcept;

    WhiteNoiseAcceleration(float T, float tilde_q) noexcept;

    WhiteNoiseAcceleration() noexcept;

    WhiteNoiseAcceleration(const WhiteNoiseAcceleration& wna);

    WhiteNoiseAcceleration(WhiteNoiseAcceleration&& wna) noexcept;

    virtual ~WhiteNoiseAcceleration() noexcept;

    WhiteNoiseAcceleration& operator=(const WhiteNoiseAcceleration& wna);

    WhiteNoiseAcceleration& operator=(WhiteNoiseAcceleration&& wna) noexcept;

    void propagate(const Eigen::Ref<const Eigen::MatrixXf>& cur_states, Eigen::Ref<Eigen::MatrixXf> prop_states) override;

    void motion(const Eigen::Ref<const Eigen::MatrixXf>& cur_states, Eigen::Ref<Eigen::MatrixXf> prop_states) override;

    Eigen::MatrixXf getNoiseSample(const int num) override;

    Eigen::MatrixXf getNoiseCovarianceMatrix() override;

    bool setProperty(const std::string& property) override { return false; };

private:
    std::mt19937_64 generator_;

    std::normal_distribution<float> distribution_;

protected:
    /**
     * Sampling interval in [time].
     */
    float T_;

    /**
     * State transition matrix.
     */
    Eigen::Matrix4f F_;

    /**
     * Convariance matrix of the additive white noise of the state model.
     */
    Eigen::Matrix4f Q_;

    /**
     * Power spectral density [length]^2/[time]^3.
     */
    float tilde_q_;

    /**
     * Square root matrix of R_.
     */
    Eigen::Matrix4f sqrt_Q_;

    /**
     * Random number generator function from a Normal distribution.
     * A call to `gauss_rnd_sample_()` returns a floating point random number.
     */
    std::function<float()> gauss_rnd_sample_;
};

#endif /* WHITENOISEACCELERATION_H */
