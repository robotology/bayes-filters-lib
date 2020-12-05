/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef FILTERINGALGORITHM_H
#define FILTERINGALGORITHM_H

#include <BayesFilters/Logger.h>

#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>


namespace bfl {
    class FilteringAlgorithm;
    typedef typename std::unordered_map<std::string, double>      FilteringParamtersD;
    typedef typename std::unordered_map<std::string, std::string> FilteringParamtersS;
}


class bfl::FilteringAlgorithm : public Logger
{
public:
    virtual ~FilteringAlgorithm() noexcept = default;

    bool boot();

    void run();

    bool wait();

    void reset();

    void reboot();

    bool teardown();

    unsigned int getFilteringStep();

    bool isRunning();

    virtual bool skip(const std::string& what_step, const bool status) = 0;

protected:
    virtual bool initialization() = 0;

    virtual void filteringStep() = 0;

    virtual bool runCondition() = 0;

private:
    unsigned int filtering_step_ = 0;

    std::thread filtering_thread_;

    void filteringRecursion();

    std::mutex mtx_run_;

    std::condition_variable cv_run_;

    bool run_ = false;

    bool reset_ = false;

    bool teardown_ = false;
};

#endif /* FILTERINGALGORITHM_H */
