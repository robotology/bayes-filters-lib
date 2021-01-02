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
#include <cmath>
#include <memory>

namespace bfl
{
namespace utils
{

/**
 * Helper type 'enable_if_t'.
 *
 * Reference: https://en.cppreference.com/w/cpp/types/enable_if#Helper_types
 */
template<bool B, class T = void >
using enable_if_t = typename std::enable_if<B,T>::type;


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
 * Return the element-wise logarithm of the sum of exponentials of the input data.
 *
 * To learn more about logsumexp, read https://en.wikipedia.org/wiki/LogSumExp.
 *
 * @param data Input numbers as a vector or a matrix.
 *
 * @return The element-wise logsumexp of the input data.
 */
template<typename Derived>
double log_sum_exp(const Eigen::MatrixBase<Derived>& data)
{
    double max = static_cast<double>(data.maxCoeff());

    return max + std::log(static_cast<double>((data.array() - max).exp().sum()));
}



/**
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
template<typename Derived, typename DerivedScalar = typename Derived::Scalar, bfl::utils::enable_if_t<std::is_floating_point<DerivedScalar>::value, bool> = true>
Eigen::Matrix<DerivedScalar, 3, Eigen::Dynamic> quaternion_to_rotation_vector(const Eigen::MatrixBase<Derived>& quaternion)
{
    Eigen::Matrix<DerivedScalar, 3, Eigen::Dynamic> rotation_vectors(3, quaternion.cols());

    for (std::size_t i = 0; i < quaternion.cols(); ++i)
    {
        const DerivedScalar norm_n = quaternion.col(i).tail(3).norm();
        if (norm_n > 1e-4)
        {
            const DerivedScalar w = quaternion.col(i)(0);
            if (w < 0)
                rotation_vectors.col(i) = - 2.0 * std::acos(-w) * quaternion.col(i).tail(3) / norm_n;
            else
                rotation_vectors.col(i) = 2.0 * std::acos(w) * quaternion.col(i).tail(3) / norm_n;
        }
        else
            rotation_vectors.col(i) = Eigen::Matrix<DerivedScalar, 3, 1>::Zero();
    }

    return rotation_vectors;
}


/**
 * Convert a matrix of rotation vectors in the tangent space (in the form (rx, ry, rz))
 * to their unitary quaternionic representation.
 *
 * Taken from
 * Chiella, A. C., Teixeira, B. O., & Pereira, G. A. (2019).
 * Quaternion-Based Robust Attitude Estimation Using an Adaptive Unscented Kalman Filter.
 * Sensors, 19(10), 2372.
 *
 * @param rotation_vector, a 3 x N matrix each column of which is a rotation vector
 *
 * @return a 4 x N matrix each column of which is the unit quaternion associated to the input rotation vector
 */
template<typename Derived, typename DerivedScalar = typename Derived::Scalar, bfl::utils::enable_if_t<std::is_floating_point<DerivedScalar>::value, bool> = true>
Eigen::Matrix<DerivedScalar, 4, Eigen::Dynamic> rotation_vector_to_quaternion(const Eigen::MatrixBase<Derived>& rotation_vector)
{
    Eigen::Matrix<DerivedScalar, 4, Eigen::Dynamic> quaternions(4, rotation_vector.cols());
    for (std::size_t i = 0; i < rotation_vector.cols(); ++i)
    {
        const DerivedScalar norm_r = rotation_vector.col(i).norm();
        if (norm_r > 1e-4)
        {
            quaternions.col(i)(0) = std::cos(norm_r / 2.0);
            quaternions.col(i).tail(3) = std::sin(norm_r / 2.0) * rotation_vector.col(i) / norm_r;
        }
        else
        {
            quaternions.col(i)(0) = 1.0;
            quaternions.col(i).tail(3) = Eigen::Matrix<DerivedScalar, 3, 1>::Zero();
        }
    }

    return quaternions;
}


/**
 * Evaluate the colwise sum between a unit quaternion (in the form (w, x, y, z) = (w, n))
 * and a set of rotation vectors (in the form (rx, ry, rz))
 *
 * Taken from
 * Chiella, A. C., Teixeira, B. O., & Pereira, G. A. (2019).
 * Quaternion-Based Robust Attitude Estimation Using an Adaptive Unscented Kalman Filter.
 * Sensors, 19(10), 2372.
 *
 * @param quaternion, a 4 x 1 matrix representing a unit quaternion
 * @param rotation_vector, a 3 x N matrix each column of which is a rotation vector
 *
 * @return a 4 x N matrix where the i-th column is the sum between the unit quaternion and the i-th rotation vector
 */
template<typename Derived, typename DerivedScalar = typename Derived::Scalar, bfl::utils::enable_if_t<std::is_floating_point<DerivedScalar>::value, bool> = true>
Eigen::Matrix<DerivedScalar, 4, Eigen::Dynamic> sum_quaternion_rotation_vector(const Eigen::MatrixBase<Derived>& quaternion, const Eigen::MatrixBase<Derived>& rotation_vector)
{
    /* Move rotation vectors to their quaternionic representation. */
    Eigen::Matrix<DerivedScalar, 4, Eigen::Dynamic> vector_as_quaternion = rotation_vector_to_quaternion(rotation_vector);

    Eigen::Quaternion<DerivedScalar> q_right(quaternion.col(0)(0), quaternion.col(0)(1), quaternion.col(0)(2), quaternion.col(0)(3));

    Eigen::Matrix<DerivedScalar, 4, Eigen::Dynamic> quaternions(4, rotation_vector.cols());
    for (std::size_t i = 0; i < rotation_vector.cols(); ++i)
    {
        Eigen::Quaternion<DerivedScalar> q_left(vector_as_quaternion.col(i)(0), vector_as_quaternion.col(i)(1), vector_as_quaternion.col(i)(2), vector_as_quaternion.col(i)(3));
        Eigen::Quaternion<DerivedScalar> sum = q_left * q_right;

        quaternions.col(i)(0) = sum.w();
        quaternions.col(i).tail(3) = sum.vec();
    }

    return quaternions;
}


/**
 * Evaluate the colwise difference between a set of unit quaternions and a unit quaternion (in the form (w, x, y, z) = (w, n))
 * in terms of rotation vectors representing the displacements in the tangent space
 *
 * Taken from
 * Chiella, A. C., Teixeira, B. O., & Pereira, G. A. (2019).
 * Quaternion-Based Robust Attitude Estimation Using an Adaptive Unscented Kalman Filter.
 * Sensors, 19(10), 2372.
 *
 * @param quaternion_left, a 4 x N matrix each column of which is a unit quaternion
 * @param quaternion_right, a 4 x 1 matrix representing a unit quaternion
 *
 * @return a 3 x N matrix where the i-th column is the difference between the i-th left quaternion and the right quaternion in the tangent space
 */
template<typename Derived, typename DerivedScalar = typename Derived::Scalar, bfl::utils::enable_if_t<std::is_floating_point<DerivedScalar>::value, bool> = true>
Eigen::Matrix<DerivedScalar, 3, Eigen::Dynamic> diff_quaternion(const Eigen::MatrixBase<Derived>& quaternion_left, const Eigen::MatrixBase<Derived>& quaternion_right)
{
    Eigen::Matrix<DerivedScalar, 4, Eigen::Dynamic> products(4, quaternion_left.cols());

    Eigen::Quaternion<DerivedScalar> q_right(quaternion_right.col(0)(0), quaternion_right.col(0)(1), quaternion_right.col(0)(2), quaternion_right.col(0)(3));
    Eigen::Quaternion<DerivedScalar> q_right_conj = q_right.conjugate();

    /* Products between each left quaternion and the conjugated right quaternion. */
    for (std::size_t i = 0; i < quaternion_left.cols(); ++i)
    {
        Eigen::Quaternion<DerivedScalar> q_left(quaternion_left.col(i)(0), quaternion_left.col(i)(1), quaternion_left.col(i)(2), quaternion_left.col(i)(3));
        Eigen::Quaternion<DerivedScalar> product = q_left * q_right_conj;

        products.col(i)(0) = product.w();
        products.col(i).tail(3) = product.vec();
    }

    /* Express displacements in the tangent space. */
    return quaternion_to_rotation_vector(products);
}


/**
 * Evaluate the weighted mean of a set of unit quaternions (in the form (w, x, y, z) = (w, n))
 *
 * Taken from
 * Chiella, A. C., Teixeira, B. O., & Pereira, G. A. (2019).
 * Quaternion-Based Robust Attitude Estimation Using an Adaptive Unscented Kalman Filter.
 * Sensors, 19(10), 2372.
 *
 * @param weight, a N x 1 matrix containing N weights
 * @param quaternion, a 4 x N matrix each column of which is a unit quaternion
 *
 * @return a 4-vector representing the weighted mean of the input quaternion set
 */
template<typename Derived, typename DerivedScalar = typename Derived::Scalar, bfl::utils::enable_if_t<std::is_floating_point<DerivedScalar>::value, bool> = true>
Eigen::Matrix<DerivedScalar, 4, 1> mean_quaternion(const Eigen::MatrixBase<Derived>& weight, const Eigen::MatrixBase<Derived>& quaternion)
{
    /* Weighted outer product of quaternions. */
    Eigen::Matrix<DerivedScalar, 4, 4> outer_product_mean = Eigen::Matrix<DerivedScalar, 4, 4>::Zero();
    for (std::size_t i = 0; i < weight.rows(); ++i)
        outer_product_mean.noalias() += weight.col(0)(i) * quaternion.col(i) * quaternion.col(i).transpose();

    /* Take the weighted mean as the eigenvector corresponding to the maximum eigenvalue. */
    Eigen::EigenSolver<Eigen::Matrix<DerivedScalar, 4, 4>> eigen_solver(outer_product_mean);
    Eigen::Matrix<std::complex<DerivedScalar>, 4, 1> eigenvalues(eigen_solver.eigenvalues());
    int maximum_index;
    eigenvalues.real().maxCoeff(&maximum_index);
    return eigen_solver.eigenvectors().real().block(0, maximum_index, 4, 1);
}


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

    /* Evaluate inv(R) */
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

    /* Evaluate V * inv(R) */
    Eigen::MatrixXd V_inv_R(V.rows(), V.cols());
    for (std::size_t i = 0; i < V.cols() / block_size; i++)
    {
        V_inv_R.middleCols(i * block_size, block_size) = V.middleCols(i * block_size, block_size) * inv_R.block(0, block_size * i, block_size, block_size);
    }

    /* Evaluate diff^{T} * inv(R) */
    Eigen::MatrixXd diff_T_inv_R(input.cols(), input.rows());
    for (std::size_t i = 0; i < num_blocks; i++)
    {
        diff_T_inv_R.middleCols(i * block_size, block_size) = diff.middleRows(i * block_size, block_size).transpose() * inv_R.block(0, block_size * i, block_size, block_size);
    }

    /* Evaluate I + V * inv(R) * U */
    Eigen::MatrixXd I_V_inv_R_U = Eigen::MatrixXd::Identity(V.rows(), V.rows()) + V_inv_R * U;

    /**
     * Evaluate diff ^{T} * inv(S) * diff
     * According to Sherman–Morrison–Woodbury formula inv(S) = inv(UV + R) = inv(R)(I - U inv(I + V inv(R) U) V inv(R))
     * See https://en.wikipedia.org/wiki/Woodbury_matrix_identity
     */
    Eigen::VectorXd weighted_diffs(input.cols());
    for (std::size_t i = 0; i < weighted_diffs.size(); i++)
        weighted_diffs(i) = diff_T_inv_R.row(i) * (Eigen::MatrixXd::Identity(U.rows(), U.rows()) - U * I_V_inv_R_U.inverse() * V_inv_R) * diff.col(i);

    /**
     * Evalute determinant(S)
     * According to Generalized matrix determinant lemma det(S) = det(UV + R) = det(R) det(I + V inv(R) U)
     * See https://en.wikipedia.org/wiki/Matrix_determinant_lemma#Generalization
     */
    double det_S;
    double det_R = 1.0;
    if (R.cols() == block_size)
        det_R = std::pow(R.determinant(), num_blocks);
    else
        for (std::size_t i = 0; i < num_blocks; i++)
            det_R *= R.block(0, block_size * i, block_size, block_size).determinant();

    det_S = det_R * I_V_inv_R_U.determinant();

    /* Evaluate the full logarithm density */
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
