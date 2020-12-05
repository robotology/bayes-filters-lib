/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/LinearModel.h>

#include <cmath>
#include <iostream>
#include <utility>

using namespace bfl;
using namespace Eigen;


LinearModel::LinearModel
(
    const double sigma_x,
    const double sigma_y,
    const unsigned int seed
) noexcept :
    generator_(std::mt19937_64(seed)),
    distribution_(std::normal_distribution<double>(0.0, 1.0)),
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


LinearModel::LinearModel(const double sigma_x, const double sigma_y) noexcept :
    LinearModel(sigma_x, sigma_y, 1)
{ }


LinearModel::LinearModel() noexcept :
    LinearModel(10.0, 10.0, 1)
{ }


LinearModel::LinearModel(const LinearModel& lin_sense) :
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
}


LinearModel::LinearModel(LinearModel&& lin_sense) noexcept :
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
        lin_sense.disable_log();

        enable_log(lin_sense.get_folder_path(), lin_sense.get_file_name_prefix());
    }
}


LinearModel& LinearModel::operator=(const LinearModel& lin_sense) noexcept
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


LinearModel& LinearModel::operator=(LinearModel&& lin_sense) noexcept
{
    if (this == &lin_sense)
        return *this;

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
        lin_sense.disable_log();

        enable_log(lin_sense.get_folder_path(), lin_sense.get_file_name_prefix());
    }

    return *this;
}


std::pair<bool, MatrixXd> LinearModel::getNoiseSample(const int num) const
{
    MatrixXd rand_vectors(2, num);
    for (int i = 0; i < rand_vectors.size(); i++)
        *(rand_vectors.data() + i) = gauss_rnd_sample_();

    MatrixXd noise_sample = sqrt_R_ * rand_vectors;

    return std::make_pair(true, std::move(noise_sample));
}


std::pair<bool, MatrixXd> LinearModel::getNoiseCovarianceMatrix() const
{
    return std::make_pair(true, R_);
}


Eigen::MatrixXd LinearModel::getMeasurementMatrix() const
{
    return H_;
}
