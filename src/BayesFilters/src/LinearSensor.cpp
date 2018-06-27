#include <BayesFilters/LinearSensor.h>

#include <cmath>
#include <iostream>
#include <utility>

using namespace bfl;
using namespace Eigen;


LinearSensor::LinearSensor(const float sigma_x, const float sigma_y, const unsigned int seed) noexcept :
    generator_(std::mt19937_64(seed)),
    distribution_(std::normal_distribution<float>(0.0, 1.0)),
    sigma_x_(sigma_x),
    sigma_y_(sigma_y),
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


LinearSensor::LinearSensor(const float sigma_x, const float sigma_y) noexcept :
    LinearSensor(sigma_x, sigma_y, 1) { }


LinearSensor::LinearSensor() noexcept :
    LinearSensor(10.0, 10.0, 1) { }


LinearSensor::~LinearSensor() noexcept
{
    if (log_enabled_)
        disableLog();
}


LinearSensor::LinearSensor(const LinearSensor& lin_sense) :
    sigma_x_(lin_sense.sigma_x_),
    sigma_y_(lin_sense.sigma_y_),
    H_(lin_sense.H_),
    R_(lin_sense.R_),
    sqrt_R_(lin_sense.sqrt_R_),
    gauss_rnd_sample_(lin_sense.gauss_rnd_sample_)
{
    if (lin_sense.log_enabled_)
    {
        std::cerr << "WARNING::LINEARSENSOR::OPERATOR=\n";
        std::cerr << "\tWARNING: Source object has log enabled, but log file stream cannot be copied. Use target object enableLog(const std::string&) to enable logging." << std::endl;
    }
};


LinearSensor::LinearSensor(LinearSensor&& lin_sense) noexcept :
    generator_(std::move(lin_sense.generator_)),
    distribution_(std::move(lin_sense.distribution_)),
    sigma_x_(lin_sense.sigma_x_),
    sigma_y_(lin_sense.sigma_y_),
    H_(std::move(lin_sense.H_)),
    R_(std::move(lin_sense.R_)),
    sqrt_R_(std::move(lin_sense.sqrt_R_)),
    gauss_rnd_sample_(std::move(lin_sense.gauss_rnd_sample_))
{
    lin_sense.sigma_x_ = 0.0;
    lin_sense.sigma_y_ = 0.0;

    if (lin_sense.log_enabled_)
    {
        lin_sense.disableLog();
        
        enableLog(lin_sense.prefix_name_);

        lin_sense.prefix_name_ = "";
    }
}


LinearSensor& LinearSensor::operator=(const LinearSensor& lin_sense) noexcept
{
    sigma_x_ = lin_sense.sigma_x_;
    sigma_y_ = lin_sense.sigma_y_;
    H_ = lin_sense.H_;
    R_ = lin_sense.R_;
    sqrt_R_ = lin_sense.sqrt_R_;

    generator_ = lin_sense.generator_;
    distribution_ = lin_sense.distribution_;
    gauss_rnd_sample_ = lin_sense.gauss_rnd_sample_;

    if (lin_sense.log_enabled_)
    {
        std::cerr << "WARNING::LINEARSENSOR::OPERATOR=\n";
        std::cerr << "\tWARNING: Source object has log enabled, but log file stream cannot be copied. Use target object enableLog(const std::string&) to enable logging." << std::endl;
    }

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

    if (lin_sense.log_enabled_)
    {
        lin_sense.disableLog();

        enableLog(lin_sense.prefix_name_);

        lin_sense.prefix_name_ = "";
    }

    return *this;
}


std::pair<bool, MatrixXf> LinearSensor::measure(const Ref<const MatrixXf>& cur_states) const
{
    MatrixXf predicted_measurements;
    std::tie(std::ignore, predicted_measurements) = predictedMeasure(cur_states);

    MatrixXf noise;
    std::tie(std::ignore, noise) = getNoiseSample(predicted_measurements.cols());

    predicted_measurements += noise;

    if (log_enabled_)
        log(predicted_measurements);

    return std::make_pair(true, predicted_measurements);
}


std::pair<bool, Eigen::MatrixXf> LinearSensor::innovation(const Eigen::Ref<const Eigen::MatrixXf>& predicted_measurements, const Eigen::Ref<const Eigen::MatrixXf>& measurements) const
{
    return std::make_pair(true, predicted_measurements.colwise() - measurements.col(0));
}


std::pair<bool, MatrixXf> LinearSensor::predictedMeasure(const Ref<const MatrixXf>& cur_states) const
{
    return std::make_pair(true, H_ * cur_states);
}


std::pair<bool, MatrixXf> LinearSensor::getNoiseSample(const int num) const
{
    MatrixXf rand_vectors(2, num);
    for (int i = 0; i < rand_vectors.size(); i++)
        *(rand_vectors.data() + i) = gauss_rnd_sample_();

    return std::make_pair(true, sqrt_R_ * rand_vectors);
}


std::pair<bool, MatrixXf> LinearSensor::getNoiseCovarianceMatrix() const
{
    return std::make_pair(true, R_);
}


void LinearSensor::enableLog(const std::string& prefix_name)
{
    prefix_name_ = prefix_name;

    log_file_measurements_.open("./" + prefix_name_ + "measurement.txt", std::ofstream::out | std::ofstream::app);

    log_enabled_ = true;
}


void LinearSensor::disableLog()
{
    log_enabled_ = false;

    log_file_measurements_.close();
}


void LinearSensor::log(const Ref<const MatrixXf>& data) const
{
    log_file_measurements_ << data.transpose() << std::endl;
}