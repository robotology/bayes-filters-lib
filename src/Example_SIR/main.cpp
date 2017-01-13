#include <iostream>
#include <memory>

#include <FilteringContext/FilteringContext.h>
#include <FilteringFunction/WhiteNoiseAcceleration.h>
#include <FilteringFunction/ParticleFilterPrediction.h>
#include <FilteringFunction/LinearSensor.h>
#include <FilteringFunction/ParticleFilterCorrection.h>
#include <FilteringFunction/Resampling.h>
#include <FilteringAlgorithm/SIRParticleFilter.h>

int main()
{
    std::shared_ptr<WhiteNoiseAcceleration>   wna(new WhiteNoiseAcceleration());
    std::shared_ptr<ParticleFilterPrediction> pf_prediction(new ParticleFilterPrediction(wna));
    std::shared_ptr<LinearSensor>             lin_sense(new LinearSensor());
    std::shared_ptr<ParticleFilterCorrection> pf_correction(new ParticleFilterCorrection(lin_sense));
    std::shared_ptr<Resampling>               resampling(new Resampling());

    std::cout << "Configuring SIR particle filter..." << std::flush;
    SIRParticleFilter sir_pf(wna, pf_prediction, lin_sense, pf_correction, resampling);
    std::cout << "done!" << std::endl;

    std::cout << "Running SIR particle filter..." << std::flush;
    sir_pf.runFilter();
    std::cout << "completed!" << std::endl;

    std::cout << "Storing filtering results..." << std::flush;
    sir_pf.getResult();
    std::cout << "done!" << std::endl;
    
    return EXIT_SUCCESS;
}
