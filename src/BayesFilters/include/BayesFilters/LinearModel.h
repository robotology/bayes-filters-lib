/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef LINEARMODEL_H
#define LINEARMODEL_H

#include <BayesFilters/LinearMeasurementModel.h>

#include <fstream>
#include <functional>
#include <random>
#include <string>

namespace bfl {
    class LinearModel;
}


class bfl::LinearModel : public LinearMeasurementModel
{
public:
    LinearModel(const double sigma_x, const double sigma_y, const unsigned int seed) noexcept;

    LinearModel(const double sigma_x, const double sigma_y) noexcept;

    LinearModel() noexcept;

    LinearModel(const LinearModel& lin_sense);

    LinearModel(LinearModel&& lin_sense) noexcept;

    virtual ~LinearModel() noexcept = default;

    LinearModel& operator=(const LinearModel& lin_sense) noexcept;

    LinearModel& operator=(LinearModel&& lin_sense) noexcept;

    std::pair<bool, Eigen::MatrixXd> getNoiseCovarianceMatrix() const override;

    Eigen::MatrixXd getMeasurementMatrix() const override;

private:
    std::mt19937_64 generator_;

    std::normal_distribution<double> distribution_;

    bool log_enabled_ = false;

    mutable std::ofstream log_file_measurements_;

protected:
    std::pair<bool, Eigen::MatrixXd> getNoiseSample(const int num) const;

    /**
     * The Sampling interval in [time].
     */
    double T_;

    /**
     * x-axis measurement noise std deviation in [length].
     */
    double sigma_x_;

    /**
     * y-axis measurement noise std deviation in [length].
     */
    double sigma_y_;

    /**
     * Measurement matrix.
     */
    Eigen::MatrixXd H_;

    /**
     * Convariance matrix of the additive white noise of the measurements.
     */
    Eigen::Matrix2d R_;

    /**
     * Square root matrix of R_.
     */
    Eigen::Matrix2d sqrt_R_;

    /**
     * Random number generator function from a Normal distribution.
     * A call to `gauss_rnd_sample_()` returns a doubleing point random number.
     */
    std::function<double()> gauss_rnd_sample_;

    std::vector<std::string> log_file_names(const std::string& folder_path, const std::string& file_name_prefix) override
    {
        return {folder_path + "/" + file_name_prefix + "_measurements"};
    }
};

#endif /* LINEARMODEL_H */
