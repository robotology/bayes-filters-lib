/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/Gaussian.h>
#include <BayesFilters/GaussianFilter.h>
#include <BayesFilters/KFPrediction.h>
#include <BayesFilters/sigma_point.h>
#include <BayesFilters/SimulatedLinearSensor.h>
#include <BayesFilters/SimulatedStateModel.h>
#include <BayesFilters/SUKFCorrection.h>
#include <BayesFilters/utils.h>
#include <BayesFilters/WhiteNoiseAcceleration.h>

#include <string>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


class MixedKFSUKFSimulation : public GaussianFilter
{
public:
    MixedKFSUKFSimulation
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
    bool runCondition() override
    {
        if (getFilteringStep() < simulation_steps_)
            return true;
        else
            return false;
    }


    bool initialization() override
    {
        return true;
    }


    void filteringStep() override
    {
        prediction_->predict(corrected_state_, predicted_state_);

        if (correction_->freeze_measurements())
            correction_->correct(predicted_state_, corrected_state_);
        else
            corrected_state_ = predicted_state_;

        log();
    }


    std::vector<std::string> log_file_names(const std::string& folder_path, const std::string& file_name_prefix) override
    {
        return  {folder_path + "/" + file_name_prefix + "_pred_mean",
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
    std::cout << "Running a SUKF filter with KF prediction and SUKF correction on a simulated target." << std::endl;

    const bool write_to_file = (argc > 1 ? std::string(argv[1]) == "ON" : false);
    if (write_to_file)
        std::cout << "Data is logged in the test folder with prefix testKF_SUKF." << std::endl;


    /* A set of parameters needed to run a Kalman filter in a simulated environment. */
    Vector4d initial_simulated_state(10.0f, 0.0f, 10.0f, 0.0f);
    std::size_t simulation_time = 100;
    /* Initialize unscented transform parameters. */
    double alpha = 1.0;
    double beta = 2.0;
    double kappa = 0.0;

    /* Step 1 - Initialization */
    std::size_t state_size = 4;
    Gaussian initial_state(state_size);
    Vector4d initial_mean(4.0, 0.04, 15.0, 0.4);
    Matrix4d initial_covariance;
    initial_covariance << pow(0.05, 2), 0,            0,            0,
                          0,            pow(0.05, 2), 0,            0,
                          0,            0,            pow(0.01, 2), 0,
                          0,            0,            0,            pow(0.01, 2);
    initial_state.mean() =  initial_mean;
    initial_state.covariance() = initial_covariance;


    /* Step 2 - Prediction */

    /* Step 2.1 - Define the state model. */

    /* Initialize a white noise acceleration state model. */
    float T = 1.0f;
    float tilde_q = 10.0f;

    std::unique_ptr<LinearStateModel> wna = utils::make_unique<WhiteNoiseAcceleration>(T, tilde_q);

    /* Step 2.2 - Define the prediction step */

    /* Initialize the Kalman filter prediction step and pass the ownership of the state model. */
    std::unique_ptr<GaussianPrediction> kf_prediction = utils::make_unique<KFPrediction>(std::move(wna));


    /* Step 3 - Correction */

    /* Step 3.1 - Define where the measurement are originated from (simulated in this case). */

    /* Initialize simulated target model with a white noise acceleration. */
    std::unique_ptr<StateModel> target_model = utils::make_unique<WhiteNoiseAcceleration>(T, tilde_q);
    std::unique_ptr<SimulatedStateModel> simulated_state_model = utils::make_unique<SimulatedStateModel>(std::move(target_model), initial_simulated_state, simulation_time);

    if (write_to_file)
        simulated_state_model->enable_log("./", "testKF_SUKF");

    /* Step 3.2 - Initialize a measurement model (a linear sensor reading x and y coordinates). */
    std::unique_ptr<LinearMeasurementModel> simulated_linear_sensor = utils::make_unique<SimulatedLinearSensor>(std::move(simulated_state_model));

    if (write_to_file)
        simulated_linear_sensor->enable_log("./", "testKF_SUKF");

    /* Step 3.3 - Initialize the serial unscented Kalman filter correction step and pass the ownership of the measurement model. */
    std::unique_ptr<GaussianCorrection> sukf_correction = utils::make_unique<SUKFCorrection>(std::move(simulated_linear_sensor), state_size, alpha, beta, kappa, 2, true);


    /* Step 4 - Assemble the serial unscented Kalman filter */
    std::cout << "Constructing mixed KF/SUKF Kalman filter..." << std::flush;

    MixedKFSUKFSimulation kf_sukf(initial_state, std::move(kf_prediction), std::move(sukf_correction), simulation_time);

    if (write_to_file)
        kf_sukf.enable_log("./", "testKF_SUKF");

    std::cout << "done!" << std::endl;


    /* Step 5 - Boot the filter */
    std::cout << "Booting mixed KF/SUKF Kalman filter..." << std::flush;

    kf_sukf.boot();

    std::cout << "completed!" << std::endl;


    /* Step 6 - Run the filter and wait until it is closed */
    /* Note that since this is a simulation, the filter will end upon simulation termination */
    std::cout << "Running mixed KF/SUKF Kalman filter..." << std::flush;

    kf_sukf.run();

    std::cout << "waiting..." << std::flush;

    if (!kf_sukf.wait())
        return EXIT_FAILURE;

    std::cout << "completed!" << std::endl;

    return EXIT_SUCCESS;
}
