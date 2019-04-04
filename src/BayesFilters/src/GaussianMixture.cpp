/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/GaussianMixture.h>

using namespace bfl;
using namespace Eigen;


GaussianMixture::GaussianMixture() :
    GaussianMixture(1, 1, 0)
{ }


GaussianMixture::GaussianMixture(const std::size_t components, const std::size_t dim) :
    GaussianMixture(components, dim, 0)
{ }


GaussianMixture::GaussianMixture
(
    const std::size_t components,
    const std::size_t dim_linear,
    const std::size_t dim_circular
) :
    components(components),
    dim(dim_linear + dim_circular),
    dim_linear(dim_linear),
    dim_circular(dim_circular),
    dim_noise(0),
    mean_(dim, components),
    covariance_(dim, dim * components),
    weight_(components)
{
    for (int i = 0; i < this->components; ++i)
        weight_(i) = 1.0 / this->components;
}


GaussianMixture::~GaussianMixture() noexcept { }


Ref<MatrixXd> GaussianMixture::mean()
{
    return mean_;
}


Ref<VectorXd> GaussianMixture::mean(const std::size_t i)
{
    return mean_.col(i);
}


double& GaussianMixture::mean(const std::size_t i, const std::size_t j)
{
    return mean_(j, i);
}


const Ref<const MatrixXd> GaussianMixture::mean() const
{
    return mean_;
}


const Ref<const VectorXd> GaussianMixture::mean(const std::size_t i) const
{
    return mean_.col(i);
}


const double& GaussianMixture::mean(const std::size_t i, const std::size_t j) const
{
    return mean_(j, i);
}


Ref<MatrixXd> GaussianMixture::covariance()
{
    return covariance_;
}


Ref<MatrixXd> GaussianMixture::covariance(const std::size_t i)
{
    return covariance_.middleCols(this->dim * i, this->dim);
}


double& GaussianMixture::covariance(const std::size_t i, const std::size_t j, const std::size_t k)
{
    return covariance_(j, (this->dim * i) + k);
}


const Ref<const MatrixXd> GaussianMixture::covariance() const
{
    return covariance_;
}


const Ref<const MatrixXd> GaussianMixture::covariance(const std::size_t i) const
{
    return covariance_.middleCols(this->dim * i, this->dim);
}


const double& GaussianMixture::covariance(const std::size_t i, const std::size_t j, const std::size_t k) const
{
    return covariance_(j, (this->dim * i) + k);
}


Ref<VectorXd> GaussianMixture::weight()
{
    return weight_;
}


double& GaussianMixture::weight(const std::size_t i)
{
    return weight_(i);
}


const Ref<const VectorXd> GaussianMixture::weight() const
{
    return weight_;
}


const double& GaussianMixture::weight(const std::size_t i) const
{
    return weight_(i);
}


bool GaussianMixture::augmentWithNoise(const Eigen::Ref<const Eigen::MatrixXd>& noise_covariance_matrix)
{
    /* Augment each state with a noise component having zero mean
       and given covariance matrix. */

    /* Check that covariance matrix is square. */
    if (noise_covariance_matrix.rows() != noise_covariance_matrix.cols())
        return false;

    dim_noise = noise_covariance_matrix.rows();
    dim += dim_noise;

    /* Add zero mean noise to each mean. */
    mean_.conservativeResize(dim, NoChange);
    mean_.bottomRows(dim_noise) = MatrixXd::Zero(dim_noise, components);

    /* Resize covariance matrix. */
    covariance_.conservativeResizeLike(MatrixXd::Zero(dim, dim * components));

    /* Move old covariance matrices from right to left to avoid aliasing.

     Note that the covariance matrix of the 0-th coomponent,
     i.e. in the top-left corner of the matrix covariance_,
     is already in the correct place.
    */
    std::size_t dim_old = dim_linear + dim_circular;
    for (std::size_t i = 0; i < (components - 1); i++)
    {
        std::size_t i_index = components - 1 - i;

        Ref<MatrixXd> new_block = covariance_.block(0, i_index * dim,     dim_old, dim_old);
        Ref<MatrixXd> old_block = covariance_.block(0, i_index * dim_old, dim_old, dim_old);

        /* Swap columns from to right to left to avoid aliasing. */
        for (std::size_t j = 0; j < dim_old; j++)
        {
            std::size_t j_index = dim_old - 1 - j;

            new_block.col(j_index).swap(old_block.col(j_index));
        }
    }

    for (std::size_t i = 0; i < components; i++)
    {
        /* Copy the noise covariance matrix in the bottom-right block of
           each covariance matrix. */
        covariance_.block(dim_old, i * dim + dim_old, dim_noise, dim_noise) = noise_covariance_matrix;

        /* Clean part of the matrix that should be zero. */
        covariance_.block(0, i * dim + dim_old, dim_old, dim_noise) = MatrixXd::Zero(dim_old, dim_noise);
    }

    return true;
}
