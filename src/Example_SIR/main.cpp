#include <iostream>

#include <FilteringContext/FilteringContext.h>
#include <FilteringAlgorithm/SIRParticleFilter.h>

int main(int argc, char** argv)
{
    std::cout << "Configuring SIR particle filter..." << std::flush;
    SIRParticleFilter sir_pf;
    sir_pf.Configure();
    std::cout << "done!" << std::endl;

    std::cout << "Configuring filtering context..." << std::flush;
    FilteringContext fc(&sir_pf);
    std::cout << "done!" << std::endl;

    std::cout << "Running SIR particle filter..." << std::flush;
    fc.run();
    std::cout << "completed!" << std::endl;

    std::cout << "Storing filtering results..." << std::flush;
    fc.saveResult();
    std::cout << "done!" << std::endl;
    
    return EXIT_SUCCESS;
}
