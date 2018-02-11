#ifndef DIRECTIONALSTATISTICSUTILS_H
#define DIRECTIONALSTATISTICSUTILS_H

#include <cmath>

#include <Eigen/Dense>


namespace bfl
{
    template <typename Derived>
    Derived directional_add(const Eigen::MatrixBase<Derived>& a, const Eigen::MatrixBase<Derived>& b)
    {
        Derived result = a + b;

        for (unsigned int i = 0; i < result.rows(); ++i)
        {
            for (unsigned int j = 0; j < result.cols(); ++j)
            {
                if      (result(i, j) >   M_PI) result(i, j) -= 2.0 * M_PI;
                else if (result(i, j) <= -M_PI) result(i, j) += 2.0 * M_PI;
            }
        }
        
        return result;
    }

    
    template <typename Derived>
    Derived directional_sub(const Eigen::MatrixBase<Derived>& a, const Eigen::MatrixBase<Derived>& b)
    {
        Derived result = a - b;

        for (unsigned int i = 0; i < result.rows(); ++i)
        {
            for (unsigned int j = 0; j < result.cols(); ++j)
            {
                if      (result(i, j) >   M_PI) result(i, j) -= 2.0 * M_PI;
                else if (result(i, j) <= -M_PI) result(i, j) += 2.0 * M_PI;
            }
        }

        return result;
    }


    template <typename DerivedA, typename DerivedW>
    double directional_mean(const Eigen::MatrixBase<DerivedA>& a, const Eigen::MatrixBase<DerivedW>& w)
    {
        Eigen::MatrixXcd complex_a(a.rows(), a.cols());

        complex_a.imag() = a.template cast<double>();

        std::complex<double> phasor = (w.array().template cast<double>() * complex_a.array().exp()).sum();

        return std::atan2(phasor.imag(), phasor.real());
    }
}

#endif /* DIRECTIONALSTATISTICSUTILS_H */
