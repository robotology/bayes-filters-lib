/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

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


VectorXd bfl::directional_statistics::directional_mean(const Ref<const MatrixXd>& a, const Ref<const VectorXd>& w)
{
    /* If one column only is provided, it is returned as is. */
    if (a.cols() == 1)
        return a.col(0);

    /* For each row i of the matrix a,
       the method computes the sum of exponentials sum(w_{k} * e^(j*a_{ik})) where j is the imaginary unit
       and then extract the phase angle using the method arg(x). */
   return ((std::complex<double>(0.0, 1.0) * a).array().exp().matrix() * w).array().arg();
}
