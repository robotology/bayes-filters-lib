#include <BayesFilters/Gaussian.h>
#include <BayesFilters/GaussianFilter.h>
#include <BayesFilters/KFCorrection.h>
#include <BayesFilters/KFPrediction.h>
#include <BayesFilters/SimulatedLinearSensor.h>
#include <BayesFilters/SimulatedStateModel.h>
#include <BayesFilters/WhiteNoiseAcceleration.h>
#include <BayesFilters/utils.h>

#include <string>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


class KFSimulation : public GaussianFilter
{
public:
    KFSimulation
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


    void log() override
    {
        logger(predicted_state_.mean().transpose(), corrected_state_.mean().transpose());
    }

private:
    std::size_t simulation_steps_;
};


int main(int argc, char* argv[])
{
    std::cout << "Running a KF filter on a simulated target." << std::endl;

    const bool write_to_file = (argc > 1 ? std::string(argv[1]) == "ON" : false);
    if (write_to_file)
        std::cout << "Data is logged in the test folder with prefix testKF." << std::endl;


    /* A set of parameters needed to run a Kalman filter in a simulated environment. */
    Vector4d initial_simulated_state(10.0f, 0.0f, 10.0f, 0.0f);
    std::size_t simulation_time = 100;


    /* Step 1 - Initialization */

    Gaussian initial_state(4);
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

    std::unique_ptr<LinearStateModel> wna = utils::make_unique<WhiteNoiseAcceleration>(T, tilde_q);

    /* Step 2.2 - Define the prediction step. */

    /* Initialize the Kalman filter prediction step and pass the ownership of the state model. */
    std::unique_ptr<KFPrediction> kf_prediction = utils::make_unique<KFPrediction>(std::move(wna));


    /* Step 3 - Correction */

    /* Step 3.1 - Define where the measurement are originated from (simulated in this case). */

    /* Initialize simulated target model with a white noise acceleration. */
    std::unique_ptr<StateModel> target_model = utils::make_unique<WhiteNoiseAcceleration>(T, tilde_q);
    std::unique_ptr<SimulatedStateModel> simulated_state_model = utils::make_unique<SimulatedStateModel>(std::move(target_model), initial_simulated_state, simulation_time);

    if (write_to_file)
        simulated_state_model->enable_log(".", "testKF");

    /* Step 3.2 - Initialize a measurement model (a linear sensor reading x and y coordinates). */
    std::unique_ptr<LinearMeasurementModel> simulated_linear_sensor = utils::make_unique<SimulatedLinearSensor>(std::move(simulated_state_model));

    if (write_to_file)
        simulated_linear_sensor->enable_log(".", "testKF");

    /* Step 3.3 - Initialize the Kalman filter correction step and pass the ownership of the measurement model. */
    std::unique_ptr<KFCorrection> kf_correction = utils::make_unique<KFCorrection>(std::move(simulated_linear_sensor));


    /* Step 4 - Assemble the Kalman filter. */
    std::cout << "Constructing Kalman filter..." << std::flush;

    KFSimulation kf(initial_state, std::move(kf_prediction), std::move(kf_correction), simulation_time);

    if (write_to_file)
        kf.enable_log(".", "testKF");

    std::cout << "done!" << std::endl;


    /* Step 5 - Boot the filter. */
    std::cout << "Booting Kalman filter..." << std::flush;

    kf.boot();

    std::cout << "completed!" << std::endl;


    /* Step 6 - Run the filter and wait until it is closed. */
    /* Note that since this is a simulation, the filter will end upon simulation termination. */
    std::cout << "Running Kalman filter..." << std::flush;

    kf.run();

    std::cout << "waiting..." << std::flush;

    if (!kf.wait())
        return EXIT_FAILURE;

    std::cout << "completed!" << std::endl;

    return EXIT_SUCCESS;
}
