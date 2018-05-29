#include <iostream>
#include <memory>

#include <BayesFilters/DrawParticles.h>
#include <BayesFilters/GaussianLikelihood.h>
#include <BayesFilters/InitSurveillanceAreaGrid.h>
#include <BayesFilters/LinearSensor.h>
#include <BayesFilters/Resampling.h>
#include <BayesFilters/SIS.h>
#include <BayesFilters/StateModelTargetMeasurement.h>
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
    bool runCondition()
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
    std::unique_ptr<StateModel> wna(new WhiteNoiseAcceleration());

    /* Step 2.2 - Define the prediction step */
    /* Initialize the particle filter prediction step and pass the ownership of the state model. */
    std::unique_ptr<PFPrediction> pf_prediction(new DrawParticles());
    pf_prediction->setStateModel(std::move(wna));


    /* Step 3 - Correction */
    /* Step 3.1 - Define the measurement model */
    /* Initialize a measurement model (a linear sensor reading x and y coordinates). */
    std::unique_ptr<MeasurementModel> lin_sense(new LinearSensor());

    /* Step 3.2 - Define where the measurement are originated from (either simulated or from a real process) */
    /* Initialize simulaterd target model, a white noise acceleration, and measurements, a MeasurementModel decoration for the linear sensor. */
    std::unique_ptr<StateModel> target_model(new WhiteNoiseAcceleration());
    std::unique_ptr<MeasurementModel> linear_target_measurement(new StateModelTargetMeasurement(std::move(lin_sense), std::move(target_model), initial_state, simulation_time));

    /* Step 3.3 - Define the correction step */
    /* Initialize the particle filter correction step and pass the ownership of the measurement model. */
    std::unique_ptr<PFCorrection> pf_correction(new UpdateParticles());
    pf_correction->setMeasurementModel(std::move(linear_target_measurement));

    /* Step 3.4 - Define the likelihood model */
    /* Initialize the the exponential likelihood, a PFCorrection decoration of the particle filter correction step. */
    std::unique_ptr<PFCorrection> exp_likelihood(new GaussianLikelihood(std::move(pf_correction)));


    /* Step 4 - Resampling */
    /* Initialize a resampling algorithm */
    std::unique_ptr<Resampling> resampling(new Resampling());


    std::cout << "Constructing SIS particle filter..." << std::flush;
    SISSimulation sis_pf(num_particle, simulation_time);
    sis_pf.setInitialization(std::move(grid_initialization));
    sis_pf.setPrediction(std::move(pf_prediction));
    sis_pf.setCorrection(std::move(exp_likelihood));
    sis_pf.setResampling(std::move(resampling));
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


    std::cout << "Saving filtering results..." << std::flush;
    sis_pf.getResult();
    std::cout << "done!" << std::endl;


    return EXIT_SUCCESS;
}
