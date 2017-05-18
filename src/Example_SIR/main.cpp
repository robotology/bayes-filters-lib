#include <iostream>
#include <memory>

#include <BayesFiltersLib/DrawParticle.h>
#include <BayesFiltersLib/LinearSensor.h>
#include <BayesFiltersLib/ParticleFilterCorrection.h>
#include <BayesFiltersLib/Resampling.h>
#include <BayesFiltersLib/SIRParticleFilter.h>
#include <BayesFiltersLib/WhiteNoiseAcceleration.h>

using namespace bfl;


int main()
{
    /* Initialize a white noise acceleration motion model */
    std::unique_ptr<WhiteNoiseAcceleration> wna(new WhiteNoiseAcceleration());

    /* Pass ownershp of the motion model to the prediction step */
    std::unique_ptr<DrawParticle> pf_prediction(new DrawParticle(std::move(wna)));


    /* Initialize a linear sensor (provides direct observation of the state) */
    std::unique_ptr<LinearSensor> lin_sense(new LinearSensor());

    /* Pass ownershp of the observation model (the sensor) to the prediction step */
    std::unique_ptr<ParticleFilterCorrection> pf_correction(new ParticleFilterCorrection(std::move(lin_sense)));


    /* Initialize a resampling algorithm */
    std::unique_ptr<Resampling> resampling(new Resampling());

    std::cout << "Configuring SIR particle filter..." << std::flush;
    SIRParticleFilter sir_pf(std::move(pf_prediction), std::move(pf_correction), std::move(resampling));
    std::cout << "done!" << std::endl;

    std::cout << "Running SIR particle filter..." << std::flush;
    sir_pf.runFilter();
    std::cout << "completed!" << std::endl;

    std::cout << "Storing filtering results..." << std::flush;
    sir_pf.getResult();
    std::cout << "done!" << std::endl;
    
    return EXIT_SUCCESS;
}
