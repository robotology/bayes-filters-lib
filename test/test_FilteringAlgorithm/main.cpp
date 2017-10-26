#include <chrono>
#include <iostream>
#include <thread>

#include <BayesFilters/FilteringAlgorithm.h>

using namespace bfl;


class DummyFilter : public FilteringAlgorithm
{
    void initialization() { std::cout << "Invoked DummyFilter::initialization()." << std::endl; };

    void filteringStep() { std::cout << "Invoked DummyFilter::filteringStep(): " << getFilteringStep() << "." << std::endl; };

    void getResult() { std::cout << "Invoked DummyFilter::getResult()." << std::endl; };

    bool runCondition() { return true; };
};


int main()
{
    std::cout << "Constructing dummy filter..." << std::flush;
    DummyFilter dummy;
    std::cout << "done!" << std::endl;

    std::cout << "Preparing dummy filter..." << std::flush;
    if (!dummy.boot())
        return EXIT_FAILURE;
    std::cout << "done!" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Running dummy filter..." << std::flush;
    dummy.run();
    std::cout << "done!" << std::endl;


    std::this_thread::sleep_for(std::chrono::milliseconds(1));


    std::cout << "Resetting dummy filter..." << std::flush;
    dummy.reset();
    std::cout << "done!" << std::endl;


    std::this_thread::sleep_for(std::chrono::milliseconds(1));


    std::cout << "Rebooting dummy filter..." << std::flush;
    dummy.reboot();
    std::cout << "done!" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Running dummy filter..." << std::flush;
    dummy.run();
    std::cout << "done!" << std::endl;


    std::this_thread::sleep_for(std::chrono::milliseconds(1));


    std::cout << "Tearing down dummy filter..." << std::flush;
    if (!dummy.teardown())
        return EXIT_FAILURE;
    std::cout << "done!" << std::endl;
    
    return EXIT_SUCCESS;
}
