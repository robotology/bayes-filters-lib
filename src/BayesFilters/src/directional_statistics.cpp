#include <BayesFilters/directional_statistics.h>

#include <cmath>

using namespace bfl;
using namespace bfl::directional_statistics;
using namespace Eigen;


MatrixXd bfl::directional_statistics::directional_add(const Ref<const MatrixXd>& a, const Ref<const VectorXd>& b)
{
    /* Let M = a.colwise() + b.
       Then Mij = arg(exp(j * Mij)) where j is the imaginary unit
       and arg(x) is the phase angle of the complex number x. */
    return (std::complex<double>(0.0, 1.0) * (a.colwise() + b)).array().exp().arg();
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
