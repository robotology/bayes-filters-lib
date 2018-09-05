#include <BayesFilters/directional_statistics.h>

#include <cmath>

using namespace bfl;
using namespace bfl::directional_statistics;
using namespace Eigen;


MatrixXd bfl::directional_statistics::directional_add(const Ref<const MatrixXd>& a, const Ref<const VectorXd>& b)
{
    MatrixXd result = a.colwise() + b;

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


MatrixXd bfl::directional_statistics::directional_sub(const Ref<const MatrixXd>& a, const Ref<const VectorXd>& b)
{
    return directional_add(a, -b);
}


double bfl::directional_statistics::directional_mean(const Ref<const VectorXd>& a, const Ref<const VectorXd>& w)
{
    MatrixXcd complex_a(a.rows(), a.cols());

    complex_a.imag() = a;

    std::complex<double> phasor = (w.array() * complex_a.array().exp()).sum();

    return std::atan2(phasor.imag(), phasor.real());
}
