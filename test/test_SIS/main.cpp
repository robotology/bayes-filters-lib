#include <iostream>
#include <memory>

#include <BayesFilters/DrawParticles.h>
#include <BayesFilters/GaussianLikelihood.h>
#include <BayesFilters/InitSurveillanceAreaGrid.h>
#include <BayesFilters/LinearSensor.h>
#include <BayesFilters/SimulatedProcess.h>
#include <BayesFilters/Resampling.h>
#include <BayesFilters/SIS.h>
#include <BayesFilters/UpdateParticles.h>
#include <BayesFilters/WhiteNoiseAcceleration.h>
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
    std::unique_ptr<ParticleSetInitialization> grid_initialization(new InitSurveillanceAreaGrid(surv_x, surv_y, num_particle_x, num_particle_y));


    /* Step 2 - Prediction */
    /* Step 2.1 - Define the state model */
    /* Initialize a white noise acceleration state model. */
    float T = 1.0f;
    float tilde_q = 10.0f;
    std::random_device rd;
    
    std::unique_ptr<StateModel> wna(new WhiteNoiseAcceleration(T, tilde_q, rd()));

    /* Step 2.2 - Define the prediction step */
    /* Initialize the particle filter prediction step and pass the ownership of the state model. */
    std::unique_ptr<PFPrediction> pf_prediction(new DrawParticles());
    pf_prediction->setStateModel(std::move(wna));


    /* Step 3 - Correction */
    /* Step 3.1 - Define the measurement model */
    /* Initialize a measurement model (a linear sensor reading x and y coordinates). */
    std::unique_ptr<MeasurementModel> lin_sense(new LinearSensor());
    lin_sense->enableLog("testSIS");

    /* Step 3.2 - Define where the measurement are originated from (either simulated or from a real process) */
    /* Initialize simulaterd target model, a white noise acceleration, and measurements, a MeasurementModel decoration for the linear sensor. */
    std::unique_ptr<StateModel> target_model(new WhiteNoiseAcceleration(T, tilde_q, rd()));
    std::unique_ptr<SimulatedProcess> simulated_process(new SimulatedProcess(std::move(target_model), initial_state, simulation_time));
    simulated_process->enableLog("testSIS");
    lin_sense->setProcess(std::move(simulated_process));

    /* Step 3.3 - Define the likelihood model */
    /* Initialize the the exponential likelihood, a PFCorrection decoration of the particle filter correction step. */
    std::unique_ptr<LikelihoodModel> exp_likelihood(new GaussianLikelihood());

    /* Step 3.4 - Define the correction step */
    /* Initialize the particle filter correction step and pass the ownership of the measurement model. */
    std::unique_ptr<PFCorrection> pf_correction(new UpdateParticles());
    pf_correction->setMeasurementModel(std::move(lin_sense));
    pf_correction->setLikelihoodModel(std::move(exp_likelihood));


    /* Step 4 - Resampling */
    /* Initialize a resampling algorithm */
    std::unique_ptr<Resampling> resampling(new Resampling());


    std::cout << "Constructing SIS particle filter..." << std::flush;
    SISSimulation sis_pf(num_particle, simulation_time);
    sis_pf.setInitialization(std::move(grid_initialization));
    sis_pf.setPrediction(std::move(pf_prediction));
    sis_pf.setCorrection(std::move(pf_correction));
    sis_pf.setResampling(std::move(resampling));
    sis_pf.enableLog("testSIS");
    std::cout << "done!" << std::endl;


    std::cout << "Booting SIS particle filter..." << std::flush;
    sis_pf.boot();
    std::cout << "completed!" << std::endl;


    std::cout << "Running SIS particle filter..." << std::flush;
    sis_pf.run();
    std::cout << "waiting..." << std::flush;
    if (!sis_pf.wait())
        return EXIT_FAILURE;
    std::cout << "completed!" << std::endl;
    

    return EXIT_SUCCESS;
}
