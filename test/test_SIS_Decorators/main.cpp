#include <iostream>
#include <memory>
#include <random>

#include <BayesFilters/DrawParticles.h>
#include <BayesFilters/GaussianLikelihood.h>
#include <BayesFilters/InitSurveillanceAreaGrid.h>
#include <BayesFilters/LinearSensor.h>
#include <BayesFilters/LikelihoodModel.h>
#include <BayesFilters/MeasurementModelDecorator.h>
#include <BayesFilters/ParticleSetInitialization.h>
#include <BayesFilters/PFCorrectionDecorator.h>
#include <BayesFilters/PFPredictionDecorator.h>
#include <BayesFilters/SimulatedStateModel.h>
#include <BayesFilters/Resampling.h>
#include <BayesFilters/StateModelDecorator.h>
#include <BayesFilters/SIS.h>
#include <BayesFilters/UpdateParticles.h>
#include <BayesFilters/WhiteNoiseAcceleration.h>
#include <BayesFilters/utils.h>
#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


class DecoratedWNA : public StateModelDecorator
{
public:
    DecoratedWNA(std::unique_ptr<StateModel> state_model) noexcept :
        StateModelDecorator(std::move(state_model)) { };


    void motion(const Eigen::Ref<const Eigen::MatrixXf>& cur_states, Eigen::Ref<Eigen::MatrixXf> mot_states) override
    {
        std::cout << "Decorator: DecoratedWNA::motion()." << std::endl;

        StateModelDecorator::motion(cur_states, mot_states);
    }
};


class DecoratedLinearSensor : public MeasurementModelDecorator
{
public:
    DecoratedLinearSensor(std::unique_ptr<MeasurementModel> observation_model) noexcept :
        MeasurementModelDecorator(std::move(observation_model)) { }


    std::pair<bool, bfl::Data> measure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) const override
    {
        std::cout << "Decorator: DecoratedLinearSensor::measure()." << std::endl;

        return MeasurementModelDecorator::measure(cur_states);
    }
};


class DecoratedDrawParticles : public PFPredictionDecorator
{
public:
    DecoratedDrawParticles(std::unique_ptr<PFPrediction> prediction) noexcept :
        PFPredictionDecorator(std::move(prediction)) { }

protected:
    void predictStep(const Eigen::Ref<const Eigen::MatrixXf>& prev_states, const Eigen::Ref<const Eigen::VectorXf>& prev_weights,
                     Eigen::Ref<Eigen::MatrixXf> pred_states, Eigen::Ref<Eigen::VectorXf> pred_weights) override
    {
        std::cout << "Decorator: DecoratedDrawParticles::predictStep()." << std::endl;

        PFPredictionDecorator::predictStep(prev_states, prev_weights, pred_states, pred_weights);
    }
};


class DecoratedUpdateParticles : public PFCorrectionDecorator
{
public:
    DecoratedUpdateParticles(std::unique_ptr<PFCorrection> correction) noexcept :
        PFCorrectionDecorator(std::move(correction)) { }

protected:
    void correctStep(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights,
                     Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) override
    {
        std::cout << "Decorator: DecoratedUpdateParticles::correctStep()." << std::endl;

        PFCorrectionDecorator::correctStep(pred_states, pred_weights, cor_states, cor_weights);
    }
};


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
    unsigned int simulation_time = 10;


    /* Step 1 - Initialization */
    /* Initialize initialization class. */
    std::unique_ptr<ParticleSetInitialization> grid_initialization = make_unique<InitSurveillanceAreaGrid>(surv_x, surv_y, num_particle_x, num_particle_y);


    /* Step 2 - Prediction */
    /* Step 2.1 - Define the state model */
    /* Initialize a white noise acceleration state model. */
    std::unique_ptr<StateModel> wna = make_unique<WhiteNoiseAcceleration>();

    /* Step 2.1.1 - Define a decoration for the state model */
    /* Initialize a white noise acceleration decorator. */
    std::unique_ptr<StateModel> decorated_wna = make_unique<DecoratedWNA>(std::move(wna));

    /* Step 2.2 - Define the prediction step */
    /* Initialize the particle filter prediction step and pass the ownership of the state model. */
    std::unique_ptr<PFPrediction> pf_prediction = make_unique<DrawParticles>();
    pf_prediction->setStateModel(std::move(decorated_wna));

    /* Step 2.2.1 - Define a decoration for the prediction step */
    /* Initialize a particle filter prediction decorator. */
    std::unique_ptr<PFPrediction> decorated_prediction = make_unique<DecoratedDrawParticles>(std::move(pf_prediction));


    /* Step 3 - Correction */
    /* Step 3.1 - Define the measurement model */
    /* Initialize a measurement model (a linear sensor reading x and y coordinates). */
    std::unique_ptr<MeasurementModel> lin_sense = make_unique<LinearSensor>();

    /* Step 3.1.1 - Define a decoration for the measurement model */
    /* Initialize a white noise acceleration decorator */
    std::unique_ptr<MeasurementModel> decorated_linearsensor = make_unique<DecoratedLinearSensor>(std::move(lin_sense));

    /* Step 3.2 - Define where the measurement are originated from (either simulated or from a real process) */
    /* Initialize simulaterd target model, a white noise acceleration, and measurements, a MeasurementModel decoration for the linear sensor. */
    std::unique_ptr<StateModel> target_model = make_unique<WhiteNoiseAcceleration>();
    std::unique_ptr<SimulatedStateModel> simulated_state_model = make_unique<SimulatedStateModel>(std::move(target_model), initial_state, simulation_time);

    /* Step 3.3 - Define the likelihood model */
    /* Initialize the the exponential likelihood, a PFCorrection decoration of the particle filter correction step. */
    std::unique_ptr<LikelihoodModel> exp_likelihood = make_unique<GaussianLikelihood>();

    /* Step 3.4 - Define the correction step */
    /* Initialize the particle filter correction step and pass the ownership of the measurement model. */
    std::unique_ptr<PFCorrection> pf_correction = make_unique<UpdateParticles>();
    pf_correction->setLikelihoodModel(std::move(exp_likelihood));
    pf_correction->setMeasurementModel(std::move(decorated_linearsensor));
    pf_correction->setProcess(std::move(simulated_state_model));

    /* Initialize a update particle decorator */
    std::unique_ptr<PFCorrection> decorated_correction = make_unique<DecoratedUpdateParticles>(std::move(pf_correction));


    /* Initialize a resampling algorithm */
    std::unique_ptr<Resampling> resampling = make_unique<Resampling>();


    std::cout << "Constructing SIS particle filter..." << std::flush;
    SISSimulation sis_pf(num_particle, simulation_time);
    sis_pf.setInitialization(std::move(grid_initialization));
    sis_pf.setPrediction(std::move(decorated_prediction));
    sis_pf.setCorrection(std::move(decorated_correction));
    sis_pf.setResampling(std::move(resampling));
    std::cout << "done!" << std::endl;


    std::cout << "Booting SIS particle filter..." << std::flush;
    sis_pf.boot();
    std::cout << "completed!" << std::endl;


    std::cout << "Running SIS particle filter..." << std::flush;
    sis_pf.run();
    std::cout << "waiting..." << std::endl;
    if (!sis_pf.wait())
        return EXIT_FAILURE;
    std::cout << "completed!" << std::endl;


    return EXIT_SUCCESS;
}
