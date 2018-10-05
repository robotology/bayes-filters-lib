#include <iostream>
#include <memory>

#include <BayesFilters/DrawParticles.h>
#include <BayesFilters/GaussianLikelihood.h>
#include <BayesFilters/InitSurveillanceAreaGrid.h>
#include <BayesFilters/SimulatedLinearSensor.h>
#include <BayesFilters/SimulatedStateModel.h>
#include <BayesFilters/Resampling.h>
#include <BayesFilters/SIS.h>
#include <BayesFilters/UpdateParticles.h>
#include <BayesFilters/WhiteNoiseAcceleration.h>
#include <BayesFilters/utils.h>
#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


class SISSimulation : public SIS
{
public:
    SISSimulation(unsigned int num_particle, unsigned int simulation_steps) noexcept :
        SIS(num_particle),
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

private:
    unsigned int simulation_steps_;
};


int main()
{
    std::cout << "Running a SIS particle filter on a simulated target." << std::endl;
    std::cout << "Data is logged in the test folder with prefix testSIS." << std::endl;

    /* A set of parameters needed to run a SIS particle filter in a simulated environment. */
    double surv_x = 1000.0;
    double surv_y = 1000.0;
    unsigned int num_particle_x = 100;
    unsigned int num_particle_y = 100;
    unsigned int num_particle = num_particle_x * num_particle_y;
    Vector4f initial_state(10.0f, 0.0f, 10.0f, 0.0f);
    unsigned int simulation_time = 100;


    /* Step 1 - Initialization */
    /* Initialize initialization class. */
    std::unique_ptr<ParticleSetInitialization> grid_initialization = utils::make_unique<InitSurveillanceAreaGrid>(surv_x, surv_y, num_particle_x, num_particle_y);


    /* Step 2 - Prediction */
    /* Step 2.1 - Define the state model */
    /* Initialize a white noise acceleration state model. */
    float T = 1.0f;
    float tilde_q = 10.0f;

    std::unique_ptr<StateModel> wna = utils::make_unique<WhiteNoiseAcceleration>(T, tilde_q);

    /* Step 2.2 - Define the prediction step */
    /* Initialize the particle filter prediction step and pass the ownership of the state model. */
    std::unique_ptr<PFPrediction> pf_prediction = utils::make_unique<DrawParticles>();
    pf_prediction->setStateModel(std::move(wna));


    /* Step 3 - Correction */
    /* Step 3.1 - Define where the measurement are originated from (either simulated or from a real process) */
    /* Initialize simulaterd target model with a white noise acceleration. */
    std::unique_ptr<StateModel> target_model = utils::make_unique<WhiteNoiseAcceleration>(T, tilde_q);
    std::unique_ptr<SimulatedStateModel> simulated_state_model = utils::make_unique<SimulatedStateModel>(std::move(target_model), initial_state, simulation_time);
    simulated_state_model->enable_log(".", "testSIS");

    /* Initialize a measurement model (a linear sensor reading x and y coordinates). */
    std::unique_ptr<MeasurementModel> simulated_linear_sensor = utils::make_unique<SimulatedLinearSensor>(std::move(simulated_state_model));
    simulated_linear_sensor->enable_log(".", "testSIS");


    /* Step 3.3 - Define the likelihood model */
    /* Initialize the the exponential likelihood, a PFCorrection decoration of the particle filter correction step. */
    std::unique_ptr<LikelihoodModel> exp_likelihood = utils::make_unique<GaussianLikelihood>();

    /* Step 3.4 - Define the correction step */
    /* Initialize the particle filter correction step and pass the ownership of the measurement model. */
    std::unique_ptr<PFCorrection> pf_correction = utils::make_unique<UpdateParticles>();
    pf_correction->setLikelihoodModel(std::move(exp_likelihood));
    pf_correction->setMeasurementModel(std::move(simulated_linear_sensor));


    /* Step 4 - Resampling */
    /* Initialize a resampling algorithm */
    std::unique_ptr<Resampling> resampling = utils::make_unique<Resampling>();


    /* Step 5 - Assemble the particle filter */
    std::cout << "Constructing SIS particle filter..." << std::flush;
    SISSimulation sis_pf(num_particle, simulation_time);
    sis_pf.setInitialization(std::move(grid_initialization));
    sis_pf.setPrediction(std::move(pf_prediction));
    sis_pf.setCorrection(std::move(pf_correction));
    sis_pf.setResampling(std::move(resampling));
    sis_pf.enable_log(".", "testSIS");
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
