#include "BayesFilters/FilteringAlgorithm.h"

#include <iostream>

using namespace bfl;


bool FilteringAlgorithm::boot()
{
    try
    {
        filtering_thread_ = std::thread(&FilteringAlgorithm::filteringRecursion, this);
    }
    catch (const std::system_error& e)
    {
        std::cerr << "ERROR::FILTERINGALGORITHM::PREAPRE" << std::endl;
        std::cerr << "ERROR::CODE:\n\t" << e.code() << std::endl;
        std::cerr << "ERROR::LOG:\n\t"  << e.what() << std::endl;
        return false;
    }

    return true;
}


void FilteringAlgorithm::run()
{
    std::lock_guard<std::mutex> lk(mtx_run_);
    run_ = true;
    cv_run_.notify_one();
}


bool FilteringAlgorithm::wait()
{
    if (filtering_thread_.joinable())
    {
        try
        {
            filtering_thread_.join();
        }
        catch (const std::system_error& e)
        {
            std::cerr << "ERROR::FILTERINGALGORITHM::WAIT" << std::endl;
            std::cerr << "ERROR::CODE:\n\t" << e.code() << std::endl;
            std::cerr << "ERROR::LOG:\n\t"  << e.what() << std::endl;
            return false;
        }
    }
    else
    {
        std::cout << "WARNING::FILTERINGALGORITHM::WAIT" << std::endl;
        std::cout << "WARNING::LOG: filtering thread is not joinable. Returning 'true'." << std::endl;
    }

    return true;
}


void FilteringAlgorithm::reset()
{
    reset_ = true;
}


void FilteringAlgorithm::reboot()
{
    std::lock_guard<std::mutex> lk(mtx_run_);
    reset_ = true;
    run_   = false;
    cv_run_.notify_one();
}


bool FilteringAlgorithm::teardown()
{
    teardown_ = true;

    std::cout << "INFO::FILTERINGALGORITHM::TEARDOWN" << std::endl;
    std::cout << "INFO::LOG: filtering thread instructed to close." << std::endl;

    return true;
}


unsigned int FilteringAlgorithm::getFilteringStep()
{
    return filtering_step_;
}


bool FilteringAlgorithm::isRunning()
{
    return run_;
}


void FilteringAlgorithm::filteringRecursion()
{
    do
    {
        reset_          = false;
        filtering_step_ = 0;

        initialization();

        std::unique_lock<std::mutex> lk(mtx_run_);
        cv_run_.wait(lk, [this]{ return (this->run_ && !this->teardown_); });
        try
        {
            lk.unlock();
        }
        catch (const std::system_error& e)
        {
            std::cerr << "ERROR::FILTERINGALGORITHM::FILTERINGRECURSION" << std::endl;
            std::cerr << "ERROR::CODE:\n\t" << e.code() << std::endl;
            std::cerr << "ERROR::LOG:\n\t"  << e.what() << std::endl;
            teardown_ = true;
        }

        while (runCondition() && !teardown_ && !reset_)
        {
            filteringStep();

            ++filtering_step_;
        }
    }
    while (runCondition() && (run_ || reset_) && !teardown_);

    run_ = false;
}
