/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

/**
 * Header-only utility library implementing missing features.
 *
 * What: Possible implementation of std::make_unique.
 * Who:  Contributed by Claudio Fantacci.
 * When: July 2001, April 2013 - May 2013, September 2018.
 * Doc:  See https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique.
 *       See also: https://herbsutter.com/gotw/_102/.
 *
 */
#ifndef UTILS_H
#define UTILS_H

#include <Eigen/Dense>

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

}
}


#endif /* UTILS_H */
