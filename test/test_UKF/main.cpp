/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/Gaussian.h>
#include <BayesFilters/GaussianFilter.h>
#include <BayesFilters/sigma_point.h>
#include <BayesFilters/SimulatedLinearSensor.h>
#include <BayesFilters/SimulatedStateModel.h>
#include <BayesFilters/UKFCorrection.h>
#include <BayesFilters/UKFPrediction.h>
#include <BayesFilters/utils.h>
#include <BayesFilters/WhiteNoiseAcceleration.h>

#include <string>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


class UKFSimulation : public GaussianFilter
{
public:
    UKFSimulation
    (
        Gaussian& initial_state,
        std::unique_ptr<GaussianPrediction> prediction,
        std::unique_ptr<GaussianCorrection> correction,
        std::size_t simulation_steps
    ) noexcept :
        GaussianFilter(std::move(prediction), std::move(correction)),
        predicted_state_(initial_state.dim_linear, initial_state.dim_circular),
        corrected_state_(initial_state),
        simulation_steps_(simulation_steps)
    { }

protected:
    bool run_condition() override
    {
        if (step_number() < simulation_steps_)
            return true;
        else
            return false;
    }


    bool initialization_step() override
    {
        return true;
    }


    void filtering_step() override
    {
        prediction().predict(corrected_state_, predicted_state_);
        correction().freeze_measurements();
        correction().correct(predicted_state_, corrected_state_);

        log();
    }


    std::vector<std::string> log_file_names(const std::string& folder_path, const std::string& file_name_prefix) override
    {
        return {folder_path + "/" + file_name_prefix + "_pred_mean",
                folder_path + "/" + file_name_prefix + "_cor_mean"};
    }


    void log() override
    {
        logger(predicted_state_.mean().transpose(), corrected_state_.mean().transpose());
    }

private:
    Gaussian predicted_state_;

    Gaussian corrected_state_;

    std::size_t simulation_steps_;
};


int main(int argc, char* argv[])
{
    std::cout << "Running a UKF filter on a simulated target." << std::endl;

    const bool write_to_file = (argc > 1 ? std::string(argv[1]) == "ON" : false);
    if (write_to_file)
        std::cout << "Data is logged in the test folder with prefix testUKF." << std::endl;


    /* A set of parameters needed to run an unscented Kalman filter in a simulated environment. */
    Vector4d initial_simulated_state(10.0f, 0.0f, 10.0f, 0.0f);
    std::size_t simulation_time = 100;
    /* Initialize unscented transform parameters. */
    double alpha = 1.0;
    double beta = 2.0;
    double kappa = 0.0;


    /* Step 1 - Initialization */

    std::size_t state_size = 4;
    Gaussian initial_state(state_size);
    Vector4d initial_mean(4.0f, 0.04f, 15.0f, 0.4f);
    Matrix4d initial_covariance;
    initial_covariance << pow(0.05, 2), 0,            0,            0,
                          0,            pow(0.05, 2), 0,            0,
                          0,            0,            pow(0.01, 2), 0,
                          0,            0,            0,            pow(0.01, 2);
    initial_state.mean() = initial_mean;
    initial_state.covariance() = initial_covariance;


    /* Step 2 - Prediction */

    /* Step 2.1 - Define the state model. */

    /* Initialize a white noise acceleration state model. */
    double T = 1.0f;
    double tilde_q = 10.0f;

    std::unique_ptr<AdditiveStateModel> wna = utils::make_unique<WhiteNoiseAcceleration>(WhiteNoiseAcceleration::Dim::TwoD, T, tilde_q);

    /* Step 2.2 - Define the prediction step. */

    /* Initialize the unscented Kalman filter prediction step and pass the ownership of the state model */
    std::unique_ptr<UKFPrediction> ukf_prediction = utils::make_unique<UKFPrediction>(std::move(wna), state_size, alpha, beta, kappa);


    /* Step 3 - Correction */

    /* Step 3.1 - Define where the measurement are originated from (simulated in this case). */

    /* Initialize simulated target model with a white noise acceleration. */
    std::unique_ptr<AdditiveStateModel> target_model = utils::make_unique<WhiteNoiseAcceleration>(WhiteNoiseAcceleration::Dim::TwoD, T, tilde_q);
    std::unique_ptr<SimulatedStateModel> simulated_state_model = utils::make_unique<SimulatedStateModel>(std::move(target_model), initial_simulated_state, simulation_time);

    if (write_to_file)
        simulated_state_model->enable_log("./", "testUKF");

    /* Step 3.2 - Initialize a measurement model (a linear sensor reading x and y coordinates). */
    double sigma_x = 10.0;
    double sigma_y = 10.0;
    Eigen::MatrixXd R(2, 2);
    R << std::pow(sigma_x, 2.0),                    0.0,
                            0.0, std::pow(sigma_y, 2.0);

    std::unique_ptr<AdditiveMeasurementModel> simulated_linear_sensor = utils::make_unique<SimulatedLinearSensor>(std::move(simulated_state_model), SimulatedLinearSensor::LinearMatrixComponent{ 4, std::vector<std::size_t>{ 0, 2 } }, R);

    if (write_to_file)
        simulated_linear_sensor->enable_log("./", "testUKF");

    /* Step 3.3 - Initialize the unscented Kalman filter correction step and pass the ownership of the measurement model. */
    std::unique_ptr<UKFCorrection> ukf_correction = utils::make_unique<UKFCorrection>(std::move(simulated_linear_sensor), state_size, alpha, beta, kappa);


    /* Step 4 - Assemble the unscented Kalman filter. */
    std::cout << "Constructing unscented Kalman filter..." << std::flush;

    UKFSimulation ukf(initial_state, std::move(ukf_prediction), std::move(ukf_correction), simulation_time);

    if (write_to_file)
        ukf.enable_log("./", "testUKF");

    std::cout << "done!" << std::endl;


    /* Step 5 - Boot the filter. */
    std::cout << "Booting unscented Kalman filter..." << std::flush;

    ukf.boot();

    std::cout << "completed!" << std::endl;


    /* Step 6 - Run the filter and wait until it is closed. */
    /* Note that since this is a simulation, the filter will end upon simulation termination. */
    std::cout << "Running unscented Kalman filter..." << std::flush;

    ukf.run();

    std::cout << "waiting..." << std::flush;

    if (!ukf.wait())
        return EXIT_FAILURE;

    std::cout << "completed!" << std::endl;

    return EXIT_SUCCESS;
}
