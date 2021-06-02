/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef LINEARMODEL_H
#define LINEARMODEL_H

#include <BayesFilters/LTIMeasurementModel.h>

#include <initializer_list>
#include <fstream>
#include <functional>
#include <random>
#include <string>

namespace bfl {
    class LinearModel;
}


class bfl::LinearModel : public LTIMeasurementModel
{
public:
    /**
     * Pair of data representing
     *  - first: the dimension of the state vector
     *  - second: the 0-based row number of the state vector that we want to measure.
     * 
     * Example:
     * LinearMatrixComponent{4, {0, 2}} results in the following measurement matrix
     * H = [ 1.0 0   0 0 ]
     *     [   0 0 1.0 0 ]
     */
    using LinearMatrixComponent = std::pair<std::size_t, std::vector<std::size_t>>;

    LinearModel(const LinearMatrixComponent& linear_matrix_component, const Eigen::Ref<const Eigen::MatrixXd>& noise_covariance_matrix, const unsigned int seed);

    LinearModel(const LinearMatrixComponent& linear_matrix_component, const Eigen::Ref<const Eigen::MatrixXd>& noise_covariance_matrix);

    virtual ~LinearModel() noexcept = default;

    std::pair<bool, Eigen::MatrixXd> getNoiseCovarianceMatrix() const override;

    Eigen::MatrixXd getMeasurementMatrix() const override;


protected:
    std::pair<bool, Eigen::MatrixXd> getNoiseSample(const int num) const;

    /**
     * The Sampling interval in [time].
     */
    double T_;

    /**
     * Square root matrix of R_.
     */
    Eigen::MatrixXd sqrt_R_;

    /**
     * Random number generator function from a Normal distribution.
     * A call to `gauss_rnd_sample_()` returns a doubleing point random number.
     */
    std::function<double()> gauss_rnd_sample_;

    std::vector<std::string> log_file_names(const std::string& folder_path, const std::string& file_name_prefix) override
    {
        return {folder_path + "/" + file_name_prefix + "_measurements"};
    }


private:
    std::mt19937_64 generator_;

    std::normal_distribution<double> distribution_;

    bool log_enabled_ = false;

    mutable std::ofstream log_file_measurements_;
};

#endif /* LINEARMODEL_H */
