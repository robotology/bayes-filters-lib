/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <iostream>
#include <memory>
#include <string>

#include <BayesFilters/BootstrapCorrection.h>
#include <BayesFilters/DrawParticles.h>
#include <BayesFilters/EstimatesExtraction.h>
#include <BayesFilters/GaussianLikelihood.h>
#include <BayesFilters/InitSurveillanceAreaGrid.h>
#include <BayesFilters/SimulatedLinearSensor.h>
#include <BayesFilters/SimulatedStateModel.h>
#include <BayesFilters/Resampling.h>
#include <BayesFilters/SIS.h>
#include <BayesFilters/WhiteNoiseAcceleration.h>
#include <BayesFilters/utils.h>
#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


class SISSimulation : public SIS
{
public:
    SISSimulation
    (
        unsigned int num_particle,
        std::size_t state_size,
        unsigned int simulation_steps,
        std::unique_ptr<ParticleSetInitialization> initialization,
        std::unique_ptr<PFPrediction> prediction,
        std::unique_ptr<PFCorrection> correction,
        std::unique_ptr<Resampling> resampling
    ) noexcept :
        SIS(num_particle, state_size, std::move(initialization), std::move(prediction), std::move(correction), std::move(resampling)),
        simulation_steps_(simulation_steps),
        estimates_extraction_(state_size)
    { }

protected:
    bool run_condition() override
    {
        if (step_number() < simulation_steps_)
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

    bool initialization_step() override
    {
        estimates_extraction_.setMethod(EstimatesExtraction::ExtractionMethod::mean);

        if (!SIS::initialization_step())
            return false;

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
    unsigned int simulation_steps_;

    EstimatesExtraction estimates_extraction_;
};


int main(int argc, char* argv[])
{
    std::cout << "Running a SIS particle filter on a simulated target." << std::endl;

    const bool write_to_file = (argc > 1 ? std::string(argv[1]) == "ON" : false);
    if (write_to_file)
        std::cout << "Data is logged in the test folder with prefix testSIS." << std::endl;


    /* A set of parameters needed to run a SIS particle filter in a simulated environment. */
    double surv_x = 1000.0;
    double surv_y = 1000.0;
    unsigned int num_particle_x = 30;
    unsigned int num_particle_y = 30;
    unsigned int num_particle = num_particle_x * num_particle_y;
    Vector4d initial_state(10.0f, 0.0f, 10.0f, 0.0f);
    unsigned int simulation_time = 100;
    std::size_t state_size = 4;

    /* Step 1 - Initialization */
    /* Initialize initialization class. */
    std::unique_ptr<ParticleSetInitialization> grid_initialization = utils::make_unique<InitSurveillanceAreaGrid>(surv_x, surv_y, num_particle_x, num_particle_y);


    /* Step 2 - Prediction */
    /* Step 2.1 - Define the state model */
    /* Initialize a white noise acceleration state model. */
    double T = 1.0f;
    double tilde_q = 10.0f;

    std::unique_ptr<LinearStateModel> wna = utils::make_unique<WhiteNoiseAcceleration>(WhiteNoiseAcceleration::Dim::TwoD, T, tilde_q);

    /* Step 2.2 - Define the prediction step */
    /* Initialize the particle filter prediction step and pass the ownership of the state model. */
    std::unique_ptr<PFPrediction> pf_prediction = utils::make_unique<DrawParticles>(std::move(wna));


    /* Step 3 - Correction */
    /* Step 3.1 - Define where the measurement are originated from (either simulated or from a real process) */
    /* Initialize simulaterd target model with a white noise acceleration. */
    std::unique_ptr<StateModel> target_model = utils::make_unique<WhiteNoiseAcceleration>(WhiteNoiseAcceleration::Dim::TwoD, T, tilde_q);
    std::unique_ptr<SimulatedStateModel> simulated_state_model = utils::make_unique<SimulatedStateModel>(std::move(target_model), initial_state, simulation_time);

    if (write_to_file)
        simulated_state_model->enable_log("./", "testSIS");

    /* Initialize a measurement model (a linear sensor reading x and y coordinates). */
    std::unique_ptr<MeasurementModel> simulated_linear_sensor = utils::make_unique<SimulatedLinearSensor>(std::move(simulated_state_model));

    if (write_to_file)
        simulated_linear_sensor->enable_log("./", "testSIS");


    /* Step 3.3 - Define the likelihood model */
    /* Initialize the the exponential likelihood, a PFCorrection decoration of the particle filter correction step. */
    std::unique_ptr<LikelihoodModel> exp_likelihood = utils::make_unique<GaussianLikelihood>();

    /* Step 3.4 - Define the correction step */
    /* Initialize the particle filter correction step and pass the ownership of the measurement model. */
    std::unique_ptr<PFCorrection> pf_correction = utils::make_unique<BootstrapCorrection>(std::move(simulated_linear_sensor), std::move(exp_likelihood));


    /* Step 4 - Resampling */
    /* Initialize a resampling algorithm */
    std::unique_ptr<Resampling> resampling = utils::make_unique<Resampling>();


    /* Step 5 - Assemble the particle filter */
    std::cout << "Constructing SIS particle filter..." << std::flush;
    SISSimulation sis_pf(num_particle, state_size, simulation_time, std::move(grid_initialization), std::move(pf_prediction), std::move(pf_correction), std::move(resampling));

    if (write_to_file)
        sis_pf.enable_log("./", "testSIS");

    std::cout << "done!" << std::endl;


    /* Step 6 - Prepare the filter to be run */
    std::cout << "Booting SIS particle filter..." << std::flush;
    sis_pf.boot();
    std::cout << "completed!" << std::endl;


    /* Step 7 - Run the filter and wait until it is closed */
    /* Note that since this is a simulation, the filter will end upon simulation termination */
    std::cout << "Running SIS particle filter..." << std::flush;
    sis_pf.run();
    std::cout << "waiting..." << std::flush;
    if (!sis_pf.wait())
        return EXIT_FAILURE;
    std::cout << "completed!" << std::endl;


    return EXIT_SUCCESS;
}
