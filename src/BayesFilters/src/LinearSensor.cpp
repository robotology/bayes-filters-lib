#include "BayesFilters/LinearSensor.h"

#include <cmath>
#include <utility>

using namespace bfl;
using namespace Eigen;


LinearSensor::LinearSensor(float sigma_x, float sigma_y, unsigned int seed) noexcept :
    sigma_x_(sigma_x),
    sigma_y_(sigma_y),
    generator_(std::mt19937_64(seed)),
    distribution_(std::normal_distribution<float>(0.0, 1.0)),
    gauss_rnd_sample_([&] { return (distribution_)(generator_); })
{
    H_.resize(2, 4);
    H_ << 1.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 1.0, 0.0;

    R_ << std::pow(sigma_x_, 2.0),                     0.0,
                              0.0, std::pow(sigma_y_, 2.0);

    sqrt_R_ << sigma_x_,      0.0,
                    0.0, sigma_y_;
}


LinearSensor::LinearSensor(float sigma_x, float sigma_y) noexcept :
    LinearSensor(sigma_x, sigma_y, 1) { }


LinearSensor::LinearSensor() noexcept :
    LinearSensor(10.0, 10.0, 1) { }


LinearSensor::~LinearSensor() noexcept { }


LinearSensor::LinearSensor(const LinearSensor& lin_sense) :
    sigma_x_(lin_sense.sigma_x_),
    sigma_y_(lin_sense.sigma_y_),
    H_(lin_sense.H_),
    R_(lin_sense.R_),
    sqrt_R_(lin_sense.sqrt_R_),
    generator_(lin_sense.generator_),
    distribution_(lin_sense.distribution_),
    gauss_rnd_sample_(lin_sense.gauss_rnd_sample_) { };


LinearSensor::LinearSensor(LinearSensor&& lin_sense) noexcept :
    sigma_x_(lin_sense.sigma_x_),
    sigma_y_(lin_sense.sigma_y_),
    H_(std::move(lin_sense.H_)),
    R_(std::move(lin_sense.R_)),
    sqrt_R_(std::move(lin_sense.sqrt_R_)),
    generator_(std::move(lin_sense.generator_)),
    distribution_(std::move(lin_sense.distribution_)),
    gauss_rnd_sample_(std::move(lin_sense.gauss_rnd_sample_))
{
    lin_sense.sigma_x_ = 0.0;
    lin_sense.sigma_y_ = 0.0;
}


LinearSensor& LinearSensor::operator=(const LinearSensor& lin_sense) noexcept
{
    LinearSensor tmp(lin_sense);
    *this = std::move(tmp);

    return *this;
}


LinearSensor& LinearSensor::operator=(LinearSensor&& lin_sense) noexcept
{
    sigma_x_ = lin_sense.sigma_x_;
    sigma_y_ = lin_sense.sigma_y_;
    H_       = std::move(lin_sense.H_);
    R_       = std::move(lin_sense.R_);
    sqrt_R_  = std::move(lin_sense.sqrt_R_);

    generator_        = std::move(lin_sense.generator_);
    distribution_     = std::move(lin_sense.distribution_);
    gauss_rnd_sample_ = std::move(lin_sense.gauss_rnd_sample_);

    lin_sense.sigma_x_ = 0.0;
    lin_sense.sigma_y_ = 0.0;

    return *this;
}


std::pair<bool, MatrixXf> LinearSensor::measure(const Ref<const MatrixXf>& cur_states)
{
    MatrixXf predicted_measurements;
    std::tie(std::ignore, predicted_measurements) = predictedMeasure(cur_states);

    MatrixXf noise;
    std::tie(std::ignore, noise) = getNoiseSample(predicted_measurements.cols());

    predicted_measurements += noise;

    return std::make_pair(true, predicted_measurements);
}


std::pair<bool, Eigen::MatrixXf> LinearSensor::innovation(const Eigen::Ref<const Eigen::MatrixXf>& predicted_measurements, const Eigen::Ref<const Eigen::MatrixXf>& measurements)
{
    return std::make_pair(true, predicted_measurements.colwise() - measurements.col(0));
}


std::pair<bool, MatrixXf> LinearSensor::predictedMeasure(const Ref<const MatrixXf>& cur_states)
{
    return std::make_pair(true, H_ * cur_states);
}


std::pair<bool, MatrixXf> LinearSensor::getNoiseSample(const int num)
{
    MatrixXf rand_vectors(2, num);
    for (int i = 0; i < rand_vectors.size(); i++)
        *(rand_vectors.data() + i) = gauss_rnd_sample_();

    return std::make_pair(true, sqrt_R_ * rand_vectors);
}


std::pair<bool, MatrixXf> LinearSensor::getNoiseCovarianceMatrix()
{
    return std::make_pair(true, R_);
}
