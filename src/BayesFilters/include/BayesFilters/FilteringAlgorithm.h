/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef FILTERINGALGORITHM_H
#define FILTERINGALGORITHM_H

#include <BayesFilters/Filter.h>
#include <BayesFilters/Logger.h>
#include <BayesFilters/Skipper.h>

#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

namespace bfl {
    class FilteringAlgorithm;
}


class bfl::FilteringAlgorithm : public Filter, public Skipper, public Logger
{
public:
    virtual ~FilteringAlgorithm() noexcept = default;

    bool boot() override;

    void run() override;

    bool wait() override;

    void reset() override;

    void reboot() override;

    bool teardown() override;

    unsigned int step_number() override;

    bool is_running() override;


protected:
    FilteringAlgorithm() = default;

    FilteringAlgorithm(const FilteringAlgorithm& filter) noexcept = delete;

    FilteringAlgorithm& operator=(const FilteringAlgorithm& filter) noexcept = delete;

    FilteringAlgorithm(FilteringAlgorithm&& filter) noexcept = delete;

    FilteringAlgorithm& operator=(FilteringAlgorithm&& filter) noexcept = delete;

    virtual bool initialization_step() = 0;

    virtual void filtering_step() = 0;

    virtual bool run_condition() = 0;


private:
    void filtering_recursion();

    unsigned int filtering_step_ = 0;

    std::thread filtering_thread_;

    std::mutex mtx_run_;

    std::condition_variable cv_run_;

    bool run_ = false;

    bool reset_ = false;

    bool teardown_ = false;
};

#endif /* FILTERINGALGORITHM_H */
