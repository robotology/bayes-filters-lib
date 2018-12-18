#include <iostream>
#include <memory>
#include <random>

#include <BayesFilters/BootstrapCorrection.h>
#include <BayesFilters/DrawParticles.h>
#include <BayesFilters/GaussianLikelihood.h>
#include <BayesFilters/InitSurveillanceAreaGrid.h>
#include <BayesFilters/SimulatedLinearSensor.h>
#include <BayesFilters/LikelihoodModel.h>
#include <BayesFilters/MeasurementModelDecorator.h>
#include <BayesFilters/ParticleSet.h>
#include <BayesFilters/ParticleSetInitialization.h>
#include <BayesFilters/PFCorrectionDecorator.h>
#include <BayesFilters/PFPredictionDecorator.h>
#include <BayesFilters/SimulatedStateModel.h>
#include <BayesFilters/Resampling.h>
#include <BayesFilters/StateModelDecorator.h>
#include <BayesFilters/SIS.h>
#include <BayesFilters/WhiteNoiseAcceleration.h>
#include <BayesFilters/utils.h>
#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


class DecoratedWNA : public StateModelDecorator
{
public:
    DecoratedWNA(std::unique_ptr<StateModel> state_model) noexcept :
        StateModelDecorator(std::move(state_model))
    { };


    void motion(const Eigen::Ref<const Eigen::MatrixXd>& cur_states, Eigen::Ref<Eigen::MatrixXd> mot_states) override
    {
        std::cout << "Decorator: DecoratedWNA::motion()." << std::endl;

        StateModelDecorator::motion(cur_states, mot_states);
    }
};


class DecoratedLinearSensor : public MeasurementModelDecorator
{
public:
    DecoratedLinearSensor(std::unique_ptr<MeasurementModel> observation_model) noexcept :
        MeasurementModelDecorator(std::move(observation_model))
    { }


    std::pair<bool, bfl::Data> measure() const override
    {
        std::cout << "Decorator: DecoratedLinearSensor::measure()." << std::endl;

        return MeasurementModelDecorator::measure();
    }
};


class DecoratedDrawParticles : public PFPredictionDecorator
{
public:
    DecoratedDrawParticles(std::unique_ptr<PFPrediction> prediction) noexcept :
        PFPredictionDecorator(std::move(prediction))
    { }

protected:
    void predictStep(const ParticleSet& prev_particles, ParticleSet& pred_particles) override
    {
        std::cout << "Decorator: DecoratedDrawParticles::predictStep()." << std::endl;

        PFPredictionDecorator::predictStep(prev_particles, pred_particles);
    }
};


class DecoratedBootstrapCorrection : public PFCorrectionDecorator
{
public:
    DecoratedBootstrapCorrection(std::unique_ptr<PFCorrection> correction) noexcept :
        PFCorrectionDecorator(std::move(correction))
    { }

protected:
    void correctStep(const ParticleSet& pred_particles, ParticleSet& cor_particles) override
    {
        std::cout << "Decorator: DecoratedBootstrapCorrection::correctStep()." << std::endl;

        PFCorrectionDecorator::correctStep(pred_particles, cor_particles);
    }
};


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
    Vector4d initial_state(10.0f, 0.0f, 10.0f, 0.0f);
    unsigned int simulation_time = 10;
    std::size_t state_size = 4;

    /* Step 1 - Initialization */
    /* Initialize initialization class. */
    std::unique_ptr<ParticleSetInitialization> grid_initialization = utils::make_unique<InitSurveillanceAreaGrid>(surv_x, surv_y, num_particle_x, num_particle_y);


    /* Step 2 - Prediction */
    /* Step 2.1 - Define the state model */
    /* Initialize a white noise acceleration state model. */
    std::unique_ptr<StateModel> wna = utils::make_unique<WhiteNoiseAcceleration>();

    /* Step 2.1.1 - Define a decoration for the state model */
    /* Initialize a white noise acceleration decorator. */
    std::unique_ptr<StateModel> decorated_wna = utils::make_unique<DecoratedWNA>(std::move(wna));

    /* Step 2.2 - Define the prediction step */
    /* Initialize the particle filter prediction step and pass the ownership of the state model. */
    std::unique_ptr<PFPrediction> pf_prediction = utils::make_unique<DrawParticles>();
    pf_prediction->setStateModel(std::move(decorated_wna));

    /* Step 2.2.1 - Define a decoration for the prediction step */
    /* Initialize a particle filter prediction decorator. */
    std::unique_ptr<PFPrediction> decorated_prediction = utils::make_unique<DecoratedDrawParticles>(std::move(pf_prediction));


    /* Step 3 - Correction */
    /* Step 3.1 - Define where the measurement are originated from (either simulated or from a real process) */
    /* Initialize simulaterd target model, a white noise acceleration, and measurements, a MeasurementModel decoration for the linear sensor. */
    std::unique_ptr<StateModel> target_model = utils::make_unique<WhiteNoiseAcceleration>();
    std::unique_ptr<SimulatedStateModel> simulated_state_model = utils::make_unique<SimulatedStateModel>(std::move(target_model), initial_state, simulation_time);

    /* Initialize a measurement model (a linear sensor reading x and y coordinates). */
    std::unique_ptr<MeasurementModel> simulated_linear_sensor = utils::make_unique<SimulatedLinearSensor>(std::move(simulated_state_model));

    /* Step 3.1.1 - Define a decoration for the measurement model */
    /* Initialize a white noise acceleration decorator */
    std::unique_ptr<MeasurementModel> decorated_linearsensor = utils::make_unique<DecoratedLinearSensor>(std::move(simulated_linear_sensor));

    /* Step 3.2 - Define the likelihood model */
    /* Initialize the the exponential likelihood, a PFCorrection decoration of the particle filter correction step. */
    std::unique_ptr<LikelihoodModel> exp_likelihood = utils::make_unique<GaussianLikelihood>();

    /* Step 3.3 - Define the correction step */
    /* Initialize the particle filter correction step and pass the ownership of the measurement model. */
    std::unique_ptr<PFCorrection> pf_correction = utils::make_unique<BoostrapCorrection>();
    pf_correction->setLikelihoodModel(std::move(exp_likelihood));
    pf_correction->setMeasurementModel(std::move(decorated_linearsensor));

    /* Initialize a update particle decorator */
    std::unique_ptr<PFCorrection> decorated_correction = utils::make_unique<DecoratedBootstrapCorrection>(std::move(pf_correction));


    /* Step 4 - Resampling */
    /* Initialize a resampling algorithm */
    std::unique_ptr<Resampling> resampling = utils::make_unique<Resampling>();


    /* Step 5 - Assemble the particle filter */
    std::cout << "Constructing SIS particle filter..." << std::flush;
    SISSimulation sis_pf(num_particle, state_size, simulation_time, std::move(grid_initialization), std::move(decorated_prediction), std::move(decorated_correction), std::move(resampling));
    std::cout << "done!" << std::endl;


    /* Step 6 - Prepare the filter to be run */
    std::cout << "Booting SIS particle filter..." << std::flush;
    sis_pf.boot();
    std::cout << "completed!" << std::endl;


    /* Step 7 - Run the filter and wait until it is closed */
    /* Note that since this is a simulation, the filter will end upon simulation termination */
    std::cout << "Running SIS particle filter..." << std::flush;
    sis_pf.run();
    std::cout << "waiting..." << std::endl;
    if (!sis_pf.wait())
        return EXIT_FAILURE;
    std::cout << "completed!" << std::endl;


    return EXIT_SUCCESS;
}
