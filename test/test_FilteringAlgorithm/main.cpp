#include <chrono>
#include <iostream>
#include <thread>

#include <BayesFilters/ParticleFilter.h>

using namespace bfl;


class DummyParticleFilter : public ParticleFilter
{
    void initialization() { std::cout << "Invoked DummyParticleFilter::initialization()." << std::endl; };

    void filteringStep() { std::cout << "Invoked DummyParticleFilter::filteringStep(): step " << getFilteringStep() << "." << std::endl; };

    void getResult() { };

    bool runCondition() { return true; };
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

    std::this_thread::sleep_for(std::chrono::seconds(1));

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

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Running dummy particle filter again..." << std::flush;
    dummy.run();
    std::cout << "done!" << std::endl;


    std::this_thread::sleep_for(std::chrono::milliseconds(1));


    std::cout << "Tearing down dummy particle filter..." << std::flush;
    if (!dummy.teardown())
        return EXIT_FAILURE;
    std::cout << "done!" << std::endl;


    return EXIT_SUCCESS;
}
