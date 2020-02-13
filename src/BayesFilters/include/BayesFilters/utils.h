/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

/**
 * Header-only utility library implementing missing features.
 *
 * Who:  Contributed by Claudio Fantacci, Nicola Piga.
 * When: July 2001, April 2013 - May 2013, September 2018, February 2020
 *
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
 * See https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique.
 * See also: https://herbsutter.com/gotw/_102/.
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
 *
 * Convert a matrix of unit quaternions (in the form (w, x, y ,z) = (w, n))
 * to their rotation vector representation in the tangent space.
 *
 * Taken from
 * Chiella, A. C., Teixeira, B. O., & Pereira, G. A. (2019).
 * Quaternion-Based Robust Attitude Estimation Using an Adaptive Unscented Kalman Filter.
 * Sensors, 19(10), 2372.
 *
 * @param quaternion, a 4 x N matrix each column of which is a unit quaternion
 *
 * @return a 3 x N matrix each column of which is the rotation vector associated to the input unit quaternion
 */
Eigen::MatrixXd quaternion_to_rotation_vector(const Eigen::Ref<const Eigen::MatrixXd>& quaternion);


/**
 * Evaluate the logarithm of a multivariate Gaussian probability density function.
 *
 * @param input Input representing the argument of the function as a vector or matrix.
 * @param mean The mean of the associated Gaussian distribution as a vector.
 * @param covariance The covariance matrix of the associated Gaussian distribution as a matrix.
 *
 * @return The value of the logarithm of the density function evaluated on the input data as a vector.
 */
template<typename Derived>
Eigen::VectorXd multivariate_gaussian_log_density(const Eigen::MatrixBase<Derived>& input, const Eigen::Ref<const Eigen::VectorXd>& mean, const Eigen::Ref<const Eigen::MatrixXd>& covariance)
{
    const auto diff = input.colwise() - mean;

    Eigen::VectorXd values(diff.cols());
    for (std::size_t i = 0; i < diff.cols(); i++)
        values(i) = - 0.5 * (static_cast<double>(diff.rows()) * std::log(2.0 * M_PI) + std::log(covariance.determinant()) + (diff.col(i).transpose() * covariance.inverse() * diff.col(i)));

    return values;
}


/**
 * Evaluate the logarithm of a multivariate Gaussian probability density function
 * using the Sherman–Morrison–Woodbury identity.
 *
 * It is assumed that the covariance matrix S can be written in the form S = UV + R
 * where R is an invertible block diagonal matrix. Each block R_{i} is square and has
 * dimension M such that N * M is the size of the input for some positive integer N.
 *
 * This version is much faster than the standard gaussian logarithm density evaluation
 * if U.cols() << U.rows().
 *
 * @param input Input representing the argument of the function as a vector or matrix.
 * @param mean The mean of the associated Gaussian distribution as a vector.
 * @param U The U factor within the covariance matrix S = UV + R of the associated Gaussian distribution as a matrix.
 * @param V The V factor within the covariance matrix S = UV + R of the associated Gaussian distribution as a matrix.
 * @param R The R summand within the covariance matrix S = UV + R of the associated Gaussian distribuion as a matrix.
 *          The matrix R has size M * (N * M) and consists in the concatenation of all the diagonal blocks R_{i}.
 *          If the diagonal blocks are all equal, it is possible to provide a single M * M block matrix.
 *
 * @return The value of the logarithm of the density function evaluated on the input data as a vector.
 */
template<typename Derived>
Eigen::VectorXd multivariate_gaussian_log_density_UVR(const Eigen::MatrixBase<Derived>& input, const Eigen::Ref<const Eigen::VectorXd>& mean, const Eigen::Ref<const Eigen::MatrixXd>& U, const Eigen::Ref<const Eigen::MatrixXd>& V, const Eigen::Ref<const Eigen::MatrixXd>& R)
{
    std::size_t input_size = input.rows();
    std::size_t block_size = R.rows();
    std::size_t num_blocks = input_size / block_size;

    const auto diff = input.colwise() - mean;

    // Evaluate inv(R)
    Eigen::MatrixXd inv_R(block_size, input_size);
    if (R.cols() == block_size)
    {
        // The diagonal blocks of R are all equal
        Eigen::MatrixXd inv_R_single = R.inverse();
        for (std::size_t i = 0; i < num_blocks; i++)
        {
            inv_R.block(0, block_size * i, block_size, block_size) = inv_R_single;
        }
    }
    else
    {
        // The diagonal blocks of R are different
        for (std::size_t i = 0; i < num_blocks; i++)
        {
            inv_R.block(0, block_size * i, block_size, block_size) = R.block(0, block_size * i, block_size, block_size).inverse();
        }
    }

    // Evaluate V * inv(R)
    Eigen::MatrixXd V_inv_R(V.rows(), V.cols());
    for (std::size_t i = 0; i < V.cols() / block_size; i++)
    {
        V_inv_R.middleCols(i * block_size, block_size) = V.middleCols(i * block_size, block_size) * inv_R.block(0, block_size * i, block_size, block_size);
    }

    // Evaluate diff^{T} * inv(R)
    Eigen::MatrixXd diff_T_inv_R(input.cols(), input.rows());
    for (std::size_t i = 0; i < num_blocks; i++)
    {
        diff_T_inv_R.middleCols(i * block_size, block_size) = diff.middleRows(i * block_size, block_size).transpose() * inv_R.block(0, block_size * i, block_size, block_size);
    }

    // Evaluate I + V * inv(R) * U
    Eigen::MatrixXd I_V_inv_R_U = Eigen::MatrixXd::Identity(V.rows(), V.rows()) + V_inv_R * U;

    // Evaluate diff ^{T} * inv(S) * diff
    // According to Sherman–Morrison–Woodbury formula inv(S) = inv(UV + R) = inv(R)(I - U inv(I + V inv(R) U) V inv(R))
    // See https://en.wikipedia.org/wiki/Woodbury_matrix_identity
    Eigen::VectorXd weighted_diffs(input.cols());
    for (std::size_t i = 0; i < weighted_diffs.size(); i++)
        weighted_diffs(i) = diff_T_inv_R.row(i) * (Eigen::MatrixXd::Identity(U.rows(), U.rows()) - U * I_V_inv_R_U.inverse() * V_inv_R) * diff.col(i);

    // Evalute determinant(S)
    // According to Generalized matrix determinant lemma det(S) = det(UV + R) = det(R) det(I + V inv(R) U)
    // See https://en.wikipedia.org/wiki/Matrix_determinant_lemma#Generalization
    double det_S;
    double det_R = 1.0;
    if (R.cols() == block_size)
        det_R = std::pow(R.determinant(), num_blocks);
    else
        for (std::size_t i = 0; i < num_blocks; i++)
            det_R *= R.block(0, block_size * i, block_size, block_size).determinant();

    det_S = det_R * I_V_inv_R_U.determinant();

    // Evaluate the full logarithm density
    Eigen::VectorXd values(input.cols());
    for (std::size_t i = 0; i < input.cols(); i++)
        values(i) = - 0.5 * (static_cast<double>(diff.rows()) * std::log(2.0 * M_PI) + std::log(det_S) + weighted_diffs(i));

    return values;
}


/**
 * Evaluate a multivariate Gaussian probability density function.
 *
 * @param input Input representing the argument of the function as a vector or matrix.
 * @param mean The mean of the associated Gaussian distribution as a vector.
 * @param covariance The covariance matrix of the associated Gaussian distribution as a matrix.
 *
 * @return The value of the density function evaluated on the input data as a vector.
 */
template<typename Derived>
Eigen::VectorXd multivariate_gaussian_density(const Eigen::MatrixBase<Derived>& input, const Eigen::Ref<const Eigen::VectorXd>& mean, const Eigen::Ref<const Eigen::MatrixXd>& covariance)
{
    return multivariate_gaussian_log_density(input, mean, covariance).array().exp();
}


/**
 * Evaluate the logarithm of a multivariate Gaussian probability density function
 * using the Sherman–Morrison–Woodbury formula.
 *
 * It is assumed that the covariance matrix S can be written in the form S = UV + R
 * where R is an invertible block diagonal matrix. Each block R_{i} is square and has
 * dimension M such that N * M is the size of the input for some positive integer N.
 *
 * This version is much faster than the standard gaussian density evaluation
 * if U.cols() << U.rows().
 *
 * @param input Input representing the argument of the function as a vector or matrix.
 * @param mean The mean of the associated Gaussian distribution as a vector.
 * @param U The U factor within the covariance matrix S = UV + R of the associated Gaussian distribution as a matrix.
 * @param V The V factor within the covariance matrix S = UV + R of the associated Gaussian distribution as a matrix.
 * @param R The R summand within the covariance matrix S = UV + R of the associated Gaussian distribuion as a matrix.
 *          The matrix R has size M * (N * M) and consists in the concatenation of all the diagonal blocks R_{i}.
 *          If the diagonal blocks are all equal, it is possible to provide a single M * M block matrix.
 *
 * @return The value of the density function evaluated on the input data as a vector.
 */
template<typename Derived>
Eigen::VectorXd multivariate_gaussian_density_UVR(const Eigen::MatrixBase<Derived>& input, const Eigen::Ref<const Eigen::VectorXd>& mean, const Eigen::Ref<const Eigen::MatrixXd>& U, const Eigen::Ref<const Eigen::MatrixXd>& V, const Eigen::Ref<const Eigen::MatrixXd>& R)
{
    return multivariate_gaussian_log_density_UVR(input, mean, U, V, R).array().exp();
}


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
