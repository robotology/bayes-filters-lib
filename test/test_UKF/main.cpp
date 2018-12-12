#include <BayesFilters/Gaussian.h>
#include <BayesFilters/GaussianFilter.h>
#include <BayesFilters/sigma_point.h>
#include <BayesFilters/SimulatedLinearSensor.h>
#include <BayesFilters/SimulatedStateModel.h>
#include <BayesFilters/UKFCorrection.h>
#include <BayesFilters/UKFPrediction.h>
#include <BayesFilters/utils.h>
#include <BayesFilters/WhiteNoiseAcceleration.h>

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
        GaussianFilter(initial_state, std::move(prediction), std::move(correction)),
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


    std::vector<std::string> log_filenames(const std::string& prefix_path, const std::string& prefix_name) override
    {
        return  {prefix_path + "/" + prefix_name + "_pred_mean",
                 prefix_path + "/" + prefix_name + "_cor_mean"};
    }


    void filteringStep() override
    {
        GaussianFilter::filteringStep();

        logger(predicted_state_.mean().transpose(), corrected_state_.mean().transpose());
    }

private:
    std::size_t simulation_steps_;
};


int main()
{
    std::cout << "Running a UKF filter on a simulated target." << std::endl;
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

    std::unique_ptr<AdditiveStateModel> wna = utils::make_unique<WhiteNoiseAcceleration>(T, tilde_q);

    /* Step 2.2 - Define the prediction step. */

    /* Initialize the unscented Kalman filter prediction step and pass the ownership of the state model. */
    std::unique_ptr<UKFPrediction> ukf_prediction = utils::make_unique<UKFPrediction>(std::move(wna), state_size, alpha, beta, kappa);


    /* Step 3 - Correction */

    /* Step 3.1 - Define where the measurement are originated from (simulated in this case). */

    /* Initialize simulated target model with a white noise acceleration. */
    std::unique_ptr<AdditiveStateModel> target_model = utils::make_unique<WhiteNoiseAcceleration>(T, tilde_q);
    std::unique_ptr<SimulatedStateModel> simulated_state_model = utils::make_unique<SimulatedStateModel>(std::move(target_model), initial_simulated_state, simulation_time);
    simulated_state_model->enable_log(".", "testUKF");

    /* Step 3.2 - Initialize a measurement model (a linear sensor reading x and y coordinates). */
    std::unique_ptr<AdditiveMeasurementModel> simulated_linear_sensor = utils::make_unique<SimulatedLinearSensor>(std::move(simulated_state_model));
    simulated_linear_sensor->enable_log(".", "testUKF");

    /* Step 3.3 - Initialize the unscented Kalman filter correction step and pass the ownership of the measurement model. */
    std::unique_ptr<UKFCorrection> ukf_correction = utils::make_unique<UKFCorrection>(std::move(simulated_linear_sensor), state_size, alpha, beta, kappa);


    /* Step 4 - Assemble the unscented Kalman filter. */
    std::cout << "Constructing unscented Kalman filter..." << std::flush;
    UKFSimulation ukf(initial_state, std::move(ukf_prediction), std::move(ukf_correction), simulation_time);
    ukf.enable_log(".", "testUKF");
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
