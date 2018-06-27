#include <chrono>
#include <iostream>
#include <thread>

#include <BayesFilters/ParticleFilter.h>

using namespace bfl;


class DummyParticleFilter : public ParticleFilter
{
    bool initialization() override
    {
        std::cout << "Invoked DummyParticleFilter::initialization()." << std::endl; return true;
    }

    void filteringStep() override
    {
        std::cout << "Invoked DummyParticleFilter::filteringStep(): step " << getFilteringStep() << "." << std::endl;
    };

    bool runCondition() override
    {
        return true;
    };
};


int main()
{
    std::cout << "Constructing dummy particle filter..." << std::flush;
    DummyParticleFilter dummy;
    std::cout << "done!" << std::endl;

    std::cout << "Preparing dummy particle filter..." << std::flush;
    if (!dummy.boot())
        return EXIT_FAILURE;
    std::cout << "done!" << std::endl;

    std::cout << "Running dummy particle filter..." << std::flush;
    dummy.run();
    std::cout << "done!" << std::endl;


    std::this_thread::sleep_for(std::chrono::milliseconds(1));


    std::cout << "Resetting dummy particle filter..." << std::flush;
    dummy.reset();
    std::cout << "done!" << std::endl;


    std::this_thread::sleep_for(std::chrono::milliseconds(1));


    std::cout << "Rebooting dummy particle filter..." << std::flush;
    dummy.reboot();
    std::cout << "done!" << std::endl;


    std::this_thread::sleep_for(std::chrono::milliseconds(100));


    std::cout << "Running dummy particle filter again..." << std::flush;
    dummy.run();
    std::cout << "done!" << std::endl;


    std::this_thread::sleep_for(std::chrono::milliseconds(1));


    std::cout << "Tearing down dummy particle filter..." << std::flush;
    if (!dummy.teardown())
        return EXIT_FAILURE;
    std::cout << "done!" << std::endl;


    std::cout << "Waiting dummy particle filter to close..." << std::flush;
    if (!dummy.wait())
        return EXIT_FAILURE;
    std::cout << "done!" << std::endl;


    return EXIT_SUCCESS;
}
