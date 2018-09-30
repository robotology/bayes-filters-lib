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


namespace bfl
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

}


#endif /* UTILS_H */
