#include <cmath>
#include <utility>

#include "BayesFilters/LinearSensor.h"

using namespace bfl;
using namespace Eigen;


LinearSensor::LinearSensor(float T, float sigma_x, float sigma_y, unsigned int seed) noexcept :
    T_(T), sigma_x_(sigma_x), sigma_y_(sigma_y),
    generator_(std::mt19937_64(seed)), distribution_(std::normal_distribution<float>(0.0, 1.0)), gauss_rnd_sample_([&] { return (distribution_)(generator_); })
{
    H_.resize(2, 4);
    H_ << 1.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 1.0, 0.0;

    R_ << std::pow(sigma_x_, 2.0),                     0.0,
                              0.0, std::pow(sigma_y_, 2.0);
}


LinearSensor::LinearSensor(float T, float sigma_x, float sigma_y) noexcept :
    LinearSensor(T, sigma_x, sigma_y, 1) { }


LinearSensor::LinearSensor() noexcept :
    LinearSensor(1.0, 10.0, 10.0, 1) { }


LinearSensor::~LinearSensor() noexcept { }


LinearSensor::LinearSensor(const LinearSensor& lin_sense) :
    T_(lin_sense.T_), sigma_x_(lin_sense.sigma_x_), sigma_y_(lin_sense.sigma_y_), H_(lin_sense.H_), R_(lin_sense.R_),
    generator_(lin_sense.generator_), distribution_(lin_sense.distribution_), gauss_rnd_sample_(lin_sense.gauss_rnd_sample_) { };


LinearSensor::LinearSensor(LinearSensor&& lin_sense) noexcept :
    T_(lin_sense.T_), sigma_x_(lin_sense.sigma_x_), sigma_y_(lin_sense.sigma_y_), H_(std::move(lin_sense.H_)), R_(std::move(lin_sense.R_)),
    generator_(std::move(lin_sense.generator_)), distribution_(std::move(lin_sense.distribution_)), gauss_rnd_sample_(std::move(lin_sense.gauss_rnd_sample_))
{
    lin_sense.T_       = 0.0;
    lin_sense.sigma_x_ = 0.0;
    lin_sense.sigma_y_ = 0.0;
}


LinearSensor& LinearSensor::operator=(const LinearSensor& lin_sense)
{
    LinearSensor tmp(lin_sense);
    *this = std::move(tmp);

    return *this;
}


LinearSensor& LinearSensor::operator=(LinearSensor&& lin_sense) noexcept
{
    T_       = lin_sense.T_;
    sigma_x_ = lin_sense.sigma_x_;
    sigma_y_ = lin_sense.sigma_y_;
    H_       = std::move(lin_sense.H_);
    R_       = std::move(lin_sense.R_);

    generator_        = std::move(lin_sense.generator_);
    distribution_     = std::move(lin_sense.distribution_);
    gauss_rnd_sample_ = std::move(lin_sense.gauss_rnd_sample_);

    lin_sense.T_       = 0.0;
    lin_sense.sigma_x_ = 0.0;
    lin_sense.sigma_y_ = 0.0;

    return *this;
}


void LinearSensor::observe(const Ref<const VectorXf>& cur_state, Ref<MatrixXf> observation)
{
    observation = H_ * cur_state;
}


void LinearSensor::noiseSample(Ref<VectorXf> sample)
{
    sample = Vector2f(sigma_x_, sigma_y_).cwiseProduct(Vector2f::NullaryExpr(2, gauss_rnd_sample_));
}


void LinearSensor::measure(const Ref<const VectorXf>& cur_state, Ref<MatrixXf> measurement)
{
    observe(cur_state, measurement);

    Vector2f sample;
    noiseSample(sample);
    measurement += sample;
}


MatrixXf LinearSensor::noiseCovariance()
{
    return R_;
}
