/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/AdditiveStateModel.h>
#include <BayesFilters/EstimatesExtraction.h>
#include <BayesFilters/GaussianLikelihood.h>
#include <BayesFilters/GPFPrediction.h>
#include <BayesFilters/GPFCorrection.h>
#include <BayesFilters/InitSurveillanceAreaGrid.h>
#include <BayesFilters/Resampling.h>
#include <BayesFilters/SimulatedLinearSensor.h>
#include <BayesFilters/SimulatedStateModel.h>
#include <BayesFilters/SIS.h>
#include <BayesFilters/UKFPrediction.h>
#include <BayesFilters/UKFCorrection.h>
#include <BayesFilters/utils.h>
#include <BayesFilters/WhiteNoiseAcceleration.h>

#include <iostream>
#include <memory>
#include <string>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


class UPFSimulation : public SIS
{
public:
    UPFSimulation
    (
        std::size_t num_particle,
        std::size_t state_size,
        std::size_t simulation_steps,
        /* Initial covariance of the Gaussian belief associated to each particle. */
        Ref<MatrixXd> initial_covariance,
        std::unique_ptr<ParticleSetInitialization> initialization,
        std::unique_ptr<PFPrediction> prediction,
        std::unique_ptr<PFCorrection> correction,
        std::unique_ptr<Resampling> resampling
    ) noexcept :
        SIS(num_particle, state_size, std::move(initialization), std::move(prediction), std::move(correction), std::move(resampling)),
        simulation_steps_(simulation_steps),
        initial_covariance_(initial_covariance),
        estimates_extraction_(state_size)
    { }

protected:
    bool runCondition() override
    {
        if (getFilteringStep() < simulation_steps_)
            return true;
        else
            return false;
    }

    std::vector<std::string> log_file_names(const std::string& folder_path, const std::string& file_name_prefix) override
    {
        std::vector<std::string> sis_filenames = SIS::log_file_names(folder_path, file_name_prefix);

        /* Add file names for logging of the conditional expected value. */
        sis_filenames.push_back(folder_path + "/" + file_name_prefix + "_mean");

        return  sis_filenames;
    }

    bool initialization() override
    {
        estimates_extraction_.setMethod(EstimatesExtraction::ExtractionMethod::mean);

        if (!SIS::initialization())
            return false;

        /* Initialize initial mean and covariance for each particle. */
        for (std::size_t i = 0; i < pred_particle_.components; i++)
        {
            /* Set mean equal to the particle position. */
            pred_particle_.mean(i) = pred_particle_.state(i);

            /* Set the covariance obtained within the ctor. */
            pred_particle_.covariance(i) = initial_covariance_;
        }

        return true;
    }

    void log() override
    {
        VectorXd mean;
        std::tie(std::ignore, mean) = estimates_extraction_.extract(cor_particle_.state(), cor_particle_.weight());

        logger(pred_particle_.state().transpose(), pred_particle_.weight().transpose(),
               cor_particle_.state().transpose(), cor_particle_.weight().transpose(),
               mean.transpose());
    }

private:
    std::size_t simulation_steps_;

    Eigen::MatrixXd initial_covariance_;

    EstimatesExtraction estimates_extraction_;
};


int main(int argc, char* argv[])
{
    std::cout << "Running an unscented particle filter on a simulated target." << std::endl;

    const bool write_to_file = (argc > 1 ? std::string(argv[1]) == "ON" : false);
    if (write_to_file)
        std::cout << "Data is logged in the test folder with prefix testUPF." << std::endl;


    /* A set of parameters needed to run an unscented particle filter in a simulated environment. */
    double surv_x = 1000.0;
    double surv_y = 1000.0;
    std::size_t num_particle_x = 7;
    std::size_t num_particle_y = 7;
    std::size_t num_particle = num_particle_x * num_particle_y;
    Vector4d initial_state(10.0f, 0.0f, 10.0f, 0.0f);
    std::size_t simulation_time = 100;
    std::size_t state_size = 4;

    /* Unscented transform parameters.*/
    double alpha = 1.0;
    double beta = 2.0;
    double kappa = 0.0;

    /* Step 1 - Initialization */

    Matrix4d initial_covariance;
    initial_covariance << pow(0.05, 2), 0,            0,            0,
                          0,            pow(0.05, 2), 0,            0,
                          0,            0,            pow(0.01, 2), 0,
                          0,            0,            0,            pow(0.01, 2);

    /* Initialize particle initialization class. */
    std::unique_ptr<ParticleSetInitialization> grid_initialization = utils::make_unique<InitSurveillanceAreaGrid>(surv_x, surv_y, num_particle_x, num_particle_y);


    /* Step 2 - Prediction */

    /* Step 2.1 - Define the state model. */

    /* Initialize a white noise acceleration state model. */
    double T = 1.0f;
    double tilde_q = 10.0f;

    std::unique_ptr<AdditiveStateModel> wna = utils::make_unique<WhiteNoiseAcceleration>(T, tilde_q);

    /* Step 2.2 - Define the prediction step */

    /* Initialize the kalman particle filter prediction step that wraps a Gaussian prediction step,
       in this case an unscented kalman filter prediction step. */
    std::unique_ptr<GaussianPrediction> upf_prediction = utils::make_unique<UKFPrediction>(std::move(wna), state_size, alpha, beta, kappa);
    std::unique_ptr<PFPrediction> gpf_prediction = utils::make_unique<GPFPrediction>(std::move(upf_prediction));


    /* Step 3 - Correction */

    /* Step 3.1 - Define where the measurement are originated from (simulated in this case). */

    /* Initialize simulated target model with a white noise acceleration. */
    std::unique_ptr<StateModel> target_model = utils::make_unique<WhiteNoiseAcceleration>(T, tilde_q);
    std::unique_ptr<SimulatedStateModel> simulated_state_model = utils::make_unique<SimulatedStateModel>(std::move(target_model), initial_state, simulation_time);

    if (write_to_file)
        simulated_state_model->enable_log("./", "testUPF");

    /* Initialize a measurement model (a linear sensor reading x and y coordinates). */
    std::unique_ptr<AdditiveMeasurementModel> simulated_linear_sensor = utils::make_unique<SimulatedLinearSensor>(std::move(simulated_state_model));

    if (write_to_file)
        simulated_linear_sensor->enable_log("./", "testUPF");


    /* Step 3.2 - Define the likelihood model. */

    /* Initialize an exponential likelihood as measurement likelihood. */
    std::unique_ptr<LikelihoodModel> exp_likelihood = utils::make_unique<GaussianLikelihood>();

    /* Step 3.3 - Define the correction step. */

    /* An additional state model is required to make the transitionProbability of the state model available
       to the particle filter correction step. */
    std::unique_ptr<StateModel> transition_probability_model = utils::make_unique<WhiteNoiseAcceleration>(T, tilde_q);

    /* Initialize the particle filter correction step that wraps a Guassian correction step,
       in this case an unscented kalman filter correction step. */
    std::unique_ptr<GaussianCorrection> upf_correction = utils::make_unique<UKFCorrection>(std::move(simulated_linear_sensor), state_size, alpha, beta, kappa);
    std::unique_ptr<PFCorrection> gpf_correction = utils::make_unique<GPFCorrection>(std::move(exp_likelihood), std::move(upf_correction), std::move(transition_probability_model));


    /* Step 4 - Resampling */

    /* Initialize a resampling algorithm. */
    std::unique_ptr<Resampling> resampling = utils::make_unique<Resampling>();


    /* Step 5 - Assemble the particle filter. */
    std::cout << "Constructing unscented particle filter..." << std::flush;

    UPFSimulation upf(num_particle, state_size, simulation_time, initial_covariance, std::move(grid_initialization), std::move(gpf_prediction), std::move(gpf_correction), std::move(resampling));

    if (write_to_file)
        upf.enable_log("./", "testUPF");

    std::cout << "done!" << std::endl;


    /* Step 6 - Prepare the filter to be run */
    std::cout << "Booting unscented particle filter..." << std::flush;

    upf.boot();

    std::cout << "completed!" << std::endl;


    /* Step 7 - Run the filter and wait until it is closed */
    /* Note that since this is a simulation, the filter will end upon simulation termination */
    std::cout << "Running unscented particle filter..." << std::flush;

    upf.run();

    std::cout << "waiting..." << std::flush;

    if (!upf.wait())
        return EXIT_FAILURE;

    std::cout << "completed!" << std::endl;

    return EXIT_SUCCESS;
}
