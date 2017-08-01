#ifndef FILTERINGALGORITHM_H
#define FILTERINGALGORITHM_H

#include <condition_variable>
#include <mutex>
#include <thread>


namespace bfl {
    class FilteringAlgorithm;
}


class bfl::FilteringAlgorithm
{
public:
    virtual ~FilteringAlgorithm() noexcept { };

    virtual void initialization() = 0;

    virtual void filteringStep() = 0;

    virtual void getResult() = 0;

    virtual bool runCondition() = 0;

    virtual bool prepare() final;

    virtual void run() final;

    virtual bool wait() final;

    virtual void reset() final;

    virtual void reboot() final;

    virtual bool teardown() final;

    virtual unsigned int getFilteringStep() final { return filtering_step_; }

private:
    void filteringRecursion();

    std::thread filtering_thread_;

    unsigned int filtering_step_ = 0;

    std::mutex mtx_run_;
    std::condition_variable cv_run_;
    bool run_      = false;

    bool reset_    = false;
    bool teardown_ = false;
};

#endif /* FILTERINGALGORITHM_H */
