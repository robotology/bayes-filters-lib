/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef SIGMAPOINT_H
#define SIGMAPOINT_H

#include <BayesFilters/AdditiveMeasurementModel.h>
#include <BayesFilters/AdditiveStateModel.h>
#include <BayesFilters/Data.h>
#include <BayesFilters/ExogenousModel.h>
#include <BayesFilters/GaussianMixture.h>
#include <BayesFilters/MeasurementModel.h>
#include <BayesFilters/StateModel.h>
#include <BayesFilters/VectorDescription.h>

#include <functional>

#include <Eigen/Dense>


namespace bfl
{
namespace sigma_point
{
    /**
     * A FunctionEvaluation return
     * - a boolean indicating if the evaluation was successful
     * - the output data in the form of bfl::Data
     * - the description of the output in the form of bfl::VectorDescription
     */
    using FunctionEvaluation = std::function<std::tuple<bool, bfl::Data, bfl::VectorDescription>(const Eigen::Ref<const Eigen::MatrixXd>&)>;

    struct UTWeight
    {
        Eigen::VectorXd mean;

        Eigen::VectorXd covariance;

        /**
         * c = sqrt(n + lambda) with lambda a ut parameter.
         */
        double c;

        /**
         * Constructs the weights from number of degrees of freedom of the input space and UT parameters alpha, beta and kappa.
         */
        UTWeight(std::size_t dof, const double alpha, const double beta, const double kappa);

        /**
         * Constructs the weights from a bfl::VectorDescription and UT parameters alpha, beta and kappa.
         * The number of degress of freedom of the input space is determined from the vector description.
         */
        UTWeight(const VectorDescription& vector_description, const double alpha, const double beta, const double kappa);
    };

    void unscented_weights(const std::size_t n, const double alpha, const double beta, const double kappa, Eigen::Ref<Eigen::VectorXd> weight_mean, Eigen::Ref<Eigen::VectorXd> weight_covariance, double& c);

    Eigen::MatrixXd sigma_point(const GaussianMixture& state, const double c);

    std::tuple<bool, GaussianMixture, Eigen::MatrixXd> unscented_transform(const GaussianMixture& input, const UTWeight& weight, FunctionEvaluation function);

    std::pair<GaussianMixture, Eigen::MatrixXd> unscented_transform(const GaussianMixture& state, const UTWeight& weight, StateModel& state_model);

    std::pair<GaussianMixture, Eigen::MatrixXd> unscented_transform(const GaussianMixture& state, const UTWeight& weight, AdditiveStateModel& state_model);

    std::tuple<bool, GaussianMixture, Eigen::MatrixXd> unscented_transform(const GaussianMixture& state, const UTWeight& weight, MeasurementModel& meas_model);

    std::tuple<bool, GaussianMixture, Eigen::MatrixXd> unscented_transform(const GaussianMixture& state, const UTWeight& weight, AdditiveMeasurementModel& meas_model);
}
}

#endif /* SIGMAPOINT_H */
