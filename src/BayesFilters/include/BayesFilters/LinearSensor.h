#ifndef LINEARSENSOR_H
#define LINEARSENSOR_H

#include <functional>
#include <random>

#include "ObservationModel.h"

namespace bfl {
    class LinearSensor;
}


class bfl::LinearSensor : public ObservationModel
{
public:
    LinearSensor(float T, float sigma_x, float sigma_y, unsigned int seed) noexcept;

    LinearSensor(float T, float sigma_x, float sigma_y) noexcept;

    LinearSensor() noexcept;

    ~LinearSensor() noexcept override;

    LinearSensor(const LinearSensor& lin_sense);

    LinearSensor(LinearSensor&& lin_sense) noexcept;

    LinearSensor& operator=(const LinearSensor& lin_sense);

    LinearSensor& operator=(LinearSensor&& lin_sense) noexcept;

    void observe(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::MatrixXf> observation) override;

    void measure(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::MatrixXf> measurement) override;

    void noiseSample(Eigen::Ref<Eigen::VectorXf> sample) override;

    Eigen::MatrixXf getNoiseCovariance() override;

    bool setProperty(const std::string property) override { return false; };

protected:
    float                           T_;                /* Sampling interval */
    float                           sigma_x_;          /* x-axis measurement noise std deviation [length] */
    float                           sigma_y_;          /* y-axis measurement noise std deviation [length] */
    Eigen::MatrixXf                 H_;                /* Measurement matrix */
    Eigen::Matrix2f                 R_;                /* Measurement white noise convariance matrix */

    std::mt19937_64                 generator_;
    std::normal_distribution<float> distribution_;
    std::function<float()>          gauss_rnd_sample_; /* Random number generator from a Normal distribution */
};

#endif /* LINEARSENSOR_HPP */
