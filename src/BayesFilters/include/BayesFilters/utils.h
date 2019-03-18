/**
 * Header-only utility library implementing missing features.
 *
 * What: Possible implementation of std::make_unique.
 * Who:  Contributed by Claudio Fantacci.
 * When: July 2001, April 2013 - May 2013, September 2018.
 * Doc:  See https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique.
 *       See also: https://herbsutter.com/gotw/_102/.
 *
 * Copyright Claudio Fantacci, 2018. All rights reserved.
 */

#ifndef UTILS_H
#define UTILS_H

#include <Eigen/Dense>

#include <chrono>
#include <memory>

namespace bfl
{
namespace utils
{

/**
 * Constructs an object of type T and wraps it in a std::unique_ptr.
 * Constructs a non-array type T. The arguments args are passed to the
 * constructor of T. This overload only participates in overload resolution if
 * T is not an array type. The function is equivalent to:
 * unique_ptr<T>(new T(std::forward<Args>(args)...))
 *
 * @note Unlike std::make_shared (which has std::allocate_shared),
 * std::make_unique does not have an allocator-aware counterpart. A hypothetical
 * allocate_unique would be required to invent the deleter type D for the
 * unique_ptr<T,D> it returns which would contain an allocator object and invoke
 * both destroy and deallocate in its operator().
 *
 * @param args list of arguments with which an instance of T will be constructed.
 *
 * @exeption May throw std::bad_alloc or any exception thrown by the constructor
 * of T. If an exception is thrown, this function has no effect.
 *
 * @return std::unique_ptr of an instance of type T.
 */
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique(Args&& ...args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


/**
 * Return the logarithm of the sum of exponentials.
 */
double log_sum_exp(const Eigen::Ref<const Eigen::VectorXd>& arguments);


/**
 * This template class provides methods to keep track of time. The default time unit is milliseconds,
 * but can be changed during object creation using a different std::chrono::duration type.
 * See https://en.cppreference.com/w/cpp/chrono/duration for reference.
 * The timer starts and stops using, respectively, start() and stop() method.
 * The get the time elapsed between a start() and stop() call, use the elapsed() method.
 * The elapsed() method can be used to get the elapsed time from the start() call, before calling stop().
 * To get the absolute time from epoch use the now() method.
 */
template<typename timetype = std::chrono::milliseconds>
class CpuTimer
{
public:
    /**
     * Start the timer.
     */
    void start()
    {
        start_time_ = std::chrono::steady_clock::now();

        running_ = true;
    }


    /**
     * Stop the timer.
     */
    void stop()
    {
        stop_time_ = std::chrono::steady_clock::now();

        running_ = false;
    }


    /**
     * Get the time passed between a start() and stop() call.
     * If used between a start() and stop() call, returns the time elapsed from the start() call.
     *
     * @return The elapsed time.
     */
    double elapsed()
    {
        std::chrono::steady_clock::duration time_span;

        if (!running_)
            time_span = stop_time_ - start_time_;
        else
            time_span = std::chrono::steady_clock::now() - start_time_;

        return static_cast<double>(time_span.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den * timetype::period::den;
    }


    /**
     * Get the absolute time from epoch.
     *
     * @return The elapsed time from epoch.
     */
    double now()
    {
        return static_cast<double>(std::chrono::steady_clock::now().time_since_epoch().count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den * timetype::period::den;
    }


    /**
     * Check if the timer is running or not.
     *
     * @return True/false if the timer is running/not running.
     */
    bool is_running()
    {
        return running_;
    }

private:
    /**
     * The start time.
     */
    std::chrono::steady_clock::time_point start_time_ = std::chrono::steady_clock::now();


    /**
     * The stop time.
     */
    std::chrono::steady_clock::time_point stop_time_ = start_time_;


    /**
     * Running status flag.
     */
    bool running_ = false;
};

}
}


#endif /* UTILS_H */
