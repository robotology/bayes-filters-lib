#include <chrono>
#include <iostream>
#include <memory>

#include <BayesFilters/DrawParticles.h>
#include <BayesFilters/LinearSensor.h>
#include <BayesFilters/Resampling.h>
#include <BayesFilters/SISParticleFilter.h>
#include <BayesFilters/UpdateParticles.h>
#include <BayesFilters/WhiteNoiseAcceleration.h>

using namespace bfl;


int main()
{
    /* Initialize a white noise acceleration motion model */
    std::unique_ptr<WhiteNoiseAcceleration> wna(new WhiteNoiseAcceleration());

    /* Pass ownership of the motion model to the prediction step */
    std::unique_ptr<DrawParticles> pf_prediction(new DrawParticles(std::move(wna)));


    /* Initialize a linear sensor (provides direct observation of the state) */
    std::unique_ptr<LinearSensor> lin_sense(new LinearSensor());

    /* Pass ownership of the observation model (the sensor) to the prediction step */
    std::unique_ptr<UpdateParticles> pf_correction(new UpdateParticles(std::move(lin_sense)));


    /* Initialize a resampling algorithm */
    std::unique_ptr<Resampling> resampling(new Resampling());


    std::cout << "Constructing SIS particle filter..." << std::flush;
    SISParticleFilter sis_pf(std::move(pf_prediction), std::move(pf_correction), std::move(resampling));
    std::cout << "done!" << std::endl;


    std::cout << "Preparing SIS particle filter..." << std::flush;
    sis_pf.prepare();
    std::cout << "completed!" << std::endl;


    std::cout << "Running SIS particle filter..." << std::flush;
    sis_pf.run();
    std::cout << "...waiting..." << std::flush;
    if (!sis_pf.wait())
        return EXIT_FAILURE;
    std::cout << "completed!" << std::endl;


    std::cout << "Storing filtering results..." << std::flush;
    sis_pf.getResult();
    std::cout << "done!" << std::endl;


    return EXIT_SUCCESS;
}
