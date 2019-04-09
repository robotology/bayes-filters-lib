/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/Gaussian.h>

#include <exception>

using namespace bfl;
using namespace Eigen;

Gaussian::Gaussian() :
    GaussianMixture(1, 1)
{ }


Gaussian::Gaussian(const std::size_t dim_linear) :
    GaussianMixture(1, dim_linear)
{ }


Gaussian::Gaussian(const std::size_t dim_linear, const std::size_t dim_circular) :
    GaussianMixture(1, dim_linear, dim_circular)
{ }


Ref<VectorXd> Gaussian::mean()
{
    return mean_.col(0);
}


const Ref<const VectorXd> Gaussian::mean() const
{
    return mean_.col(0);
}


double& Gaussian::mean(const std::size_t i)
{
    return mean_(i, 0);
}


const double& Gaussian::mean(const std::size_t i) const
{
    return mean_(i, 0);
}


Ref<MatrixXd> Gaussian::covariance()
{
    return covariance_;
}


const Ref<const MatrixXd> Gaussian::covariance() const
{
    return covariance_;
}


double& Gaussian::covariance(const std::size_t i, const std::size_t j)
{

    return covariance_(i, j);
}


const double& Gaussian::covariance(const std::size_t i, const std::size_t j) const
{
    return covariance_(i, j);
}


double& Gaussian::weight()
{
    return weight_(0);
}


const double& Gaussian::weight() const
{
    return weight_(0);
}
