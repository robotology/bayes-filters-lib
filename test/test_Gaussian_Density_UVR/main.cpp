/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/Gaussian.h>
#include <BayesFilters/GaussianMixture.h>
#include <BayesFilters/AdditiveMeasurementModel.h>
#include <BayesFilters/sigma_point.h>
#include <BayesFilters/utils.h>

#include <Eigen/Dense>

#include <iostream>

using namespace Eigen;
using namespace bfl;


class SimulatedMeasurement : public AdditiveMeasurementModel
{
public:
    SimulatedMeasurement(const Ref<VectorXd>& measurement, const Ref<VectorXd>& predicted_measurement) :
        measurement_(measurement), predicted_measurement_(predicted_measurement)
    { }


    bool freeze(const Data& data) override
    {
        return true;
    }


    std::pair<bool, Data> measure(const Data& data = Data()) const override
    {
        MatrixXd measurement(measurement_.size(), 1);
        measurement.col(0) = measurement_;
        return std::make_pair(true, measurement);
    }


    std::pair<bool, Data> predictedMeasure(const Ref<const MatrixXd>& cur_states) const override
    {
        MatrixXd predicted = MatrixXd::Zero(measurement_.size(), cur_states.cols());
        predicted.colwise() += predicted_measurement_;
        return std::make_pair(true, predicted);
    }


    std::pair<bool, Data> innovation(const Data& predicted_measurements, const Data& measurements) const override
    {
        MatrixXd innovation = -(any::any_cast<MatrixXd>(predicted_measurements).colwise() - any::any_cast<MatrixXd>(measurements).col(0));

        return std::make_pair(true, std::move(innovation));
    }


    std::pair<bool, MatrixXd> getNoiseCovarianceMatrix() const
    {
        VectorXd covariance(measurement_.size());
        for (std::size_t i = 0; i < measurement_.size(); i++)
            covariance(i) = 0.1 * (i + 1);

        MatrixXd covariance_matrix = covariance.asDiagonal();

        return std::make_pair(true, covariance_matrix);
    }


    VectorDescription getInputDescription() const override
    {
        return VectorDescription(3, 0, measurement_.size());
    }


    VectorDescription getMeasurementDescription() const override
    {
        return VectorDescription(measurement_.size());
    }

private:
    VectorXd measurement_;

    VectorXd predicted_measurement_;
};


int main()
{
    std::cout << "[Test] " << std::endl;
    {
        std::cout << "Constructing a simulated scenario..." << std::endl;

        // Predicted state
        Gaussian predicted_state(3);
        Matrix3d covariance;
        covariance << 0.01, 0.0,  0.0,
                      0.0,  0.01, 0.0,
                      0.0,  0.0,  0.01;
        predicted_state.mean() = Vector3d::Zero();
        predicted_state.covariance() = covariance;

        // Measurement model
        VectorXd measurement(6);
        measurement(0) = 1.05;
        measurement(1) = -0.05;
        measurement(2) = 0.05;
        measurement(3) = 0.95;
        measurement(4) = 1.1;
        measurement(5) = 0.8;

        VectorXd predicted(6);
        predicted(0) = 1.0;
        predicted(1) = 0.0;
        predicted(2) = 0.0;
        predicted(3) = 1.0;
        predicted(4) = 1.0;
        predicted(5) = 1.0;

        SimulatedMeasurement measurement_model(measurement, predicted);

        // Unscented Transform parameters
        double alpha = 1.0;
        double beta = 2.0;
        double kappa = 0.0;
        sigma_point::UTWeight ut_weight(measurement_model.getInputDescription().noiseless_description(), alpha, beta, kappa);

        // Propagate belief
        GaussianMixture predicted_measurement(1, 6);
        MatrixXd cross_covariance;
        std::tie(std::ignore, predicted_measurement, cross_covariance) = sigma_point::unscented_transform(predicted_state, ut_weight, measurement_model);

        // Extract mean and predicted measurement covariance
        VectorXd my = predicted_measurement.mean(0);
        MatrixXd Py = predicted_measurement.covariance(0);

        std::cout << "done." << std::endl;

        // Evaluate likelihood using standard Gaussian
        VectorXd likelihood_0 = utils::multivariate_gaussian_density(measurement, my, Py);

        std::cout << "Evaluated likelihood using standard Gaussian evaluation is " << likelihood_0(0) << std::endl;

        // Evaluate likelihood using Gaussian having covariance in the form S = UV + R with R block diagonal

        // Evaluate the predicted measurement covariance using the form Py = YY^{T} + R with U = Y, V = Y^{T}
        MatrixXd input_sigma_points = sigma_point::sigma_point(predicted_state, ut_weight.c);

        bfl::Data prediction;
        std::tie(std::ignore, prediction) = measurement_model.predictedMeasure(input_sigma_points);
        MatrixXd propagated_sigma_points = bfl::any::any_cast<MatrixXd&&>(std::move(prediction));

        MatrixXd sqrt_ut_weight = ut_weight.covariance.array().sqrt().matrix().asDiagonal();
        MatrixXd Y = propagated_sigma_points.colwise() - my;
        Y *= sqrt_ut_weight;

        MatrixXd R_full;
        std::tie(std::ignore, R_full) = measurement_model.getNoiseCovarianceMatrix();

        // Compose the R matrix with adjacent diagonal blocks, as required by the method
        // utils::multivariate_gaussian_UVR()
        MatrixXd R(2, 6);
        for (std::size_t i = 0; i < R_full.cols() / 2; i++)
            R.block<2, 2>(0, 2 * i) = R_full.block<2, 2>(2 * i, 2 * i);

        VectorXd likelihood_1 = utils::multivariate_gaussian_density_UVR(measurement, my, Y, Y.transpose(), R);
        std::cout << "Evaluated likelihood using Gaussian with UVR factorized covariance matrix is " << likelihood_1(0) << std::endl;

        if (!(std::abs(likelihood_0(0) - likelihood_1(0)) < 0.00001))
        {
            std::cerr << "Evaluation of likelihood using Gaussian with UVR factorized covariance matrix failed."
                      << "Should be " << likelihood_0(0) << ", is " << likelihood_1(0) << std::endl;
            return EXIT_FAILURE;
        }
    }

    std::cout << "done." << std::endl;

    return EXIT_SUCCESS;
}
