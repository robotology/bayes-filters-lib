/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/GaussianMixture.h>

using namespace bfl;
using namespace Eigen;


GaussianMixture::GaussianMixture() noexcept:
    GaussianMixture(1, 1, 0, false)
{ }


GaussianMixture::GaussianMixture(const std::size_t components, const std::size_t dim) noexcept :
    GaussianMixture(components, dim, 0, false)
{ }


GaussianMixture::GaussianMixture
(
    const std::size_t components,
    const std::size_t dim_linear,
    const std::size_t dim_circular,
    const bool use_quaternion
) noexcept :
    components(components),
    use_quaternion(use_quaternion),
    dim_circular_component(use_quaternion ? 4 : 1),
    dim(dim_linear + dim_circular * dim_circular_component),
    dim_linear(dim_linear),
    dim_circular(dim_circular),
    dim_noise(0),
    dim_covariance(use_quaternion ? dim_linear + dim_circular * (dim_circular_component - 1) : dim),
    mean_(dim, components),
    covariance_(dim_covariance, dim_covariance * components),
    weight_(components)
{
    for (int i = 0; i < this->components; ++i)
        weight_(i) = 1.0 / this->components;

    /* Note:
       When using use_quaternion == false, the hypothesis is that there are dim_circular
       independent states each belonging to the manifold S1 (i.e. dim_circular angles).
       In this implementation they are treated as belonging to R^(dim_circular).
       Hence, the size of the covariance matrix is dim_covariance x (dim_covariance * components)
       where dim_covariance = dim.

       When using use_quaternion == true, instead the hypothesis is that there are dim_circular
       quaternions in the state, each belonging to the manifold S3.
       In this case, dim_circular_component = 4, since they are represented using 4 numbers.
       However, the covariance is represented using rotation vectors in R^3 that belong to the tangent
       space of the quaternion manifold. Hence, the size of the covariance matrix is
       dim_covariance x (dim_covariance * components) where dim_covariance = dim_linear + dim_circular * 3.
    */

}


GaussianMixture::~GaussianMixture() noexcept
{ }


void GaussianMixture::resize(const std::size_t components, const std::size_t dim_linear, const std::size_t dim_circular)
{
    std::size_t new_dim = dim_linear + dim_circular * dim_circular_component;
    std::size_t new_dim_covariance = use_quaternion ? dim_linear + dim_circular * (dim_circular_component - 1) : new_dim;

    if ((this->dim_linear == dim_linear) && (this->dim_circular == dim_circular) && (this->components == components))
        return;
    else if ((this->dim == new_dim) && (this->components != components))
    {
        mean_.conservativeResize(NoChange, components);
        covariance_.conservativeResize(NoChange, dim_covariance * components);
        weight_.conservativeResize(components);
    }
    else
    {
        // In any other case, it does not make sense to do conservative resize
        // since either old data is truncated or new data is incomplete
        mean_.resize(new_dim, components);
        covariance_.resize(new_dim_covariance, new_dim_covariance * components);
        weight_.resize(components);
    }

    this->components = components;
    this->dim = new_dim;
    this->dim_covariance = new_dim_covariance;
    this->dim_linear = dim_linear;
    this->dim_circular = dim_circular;
}


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
    return covariance_.middleCols(this->dim_covariance * i, this->dim_covariance);
}


double& GaussianMixture::covariance(const std::size_t i, const std::size_t j, const std::size_t k)
{
    return covariance_(j, (this->dim_covariance * i) + k);
}


const Ref<const MatrixXd> GaussianMixture::covariance() const
{
    return covariance_;
}


const Ref<const MatrixXd> GaussianMixture::covariance(const std::size_t i) const
{
    return covariance_.middleCols(this->dim_covariance * i, this->dim_covariance);
}


const double& GaussianMixture::covariance(const std::size_t i, const std::size_t j, const std::size_t k) const
{
    return covariance_(j, (this->dim_covariance * i) + k);
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
    dim_covariance += dim_noise;

    /* Add zero mean noise to each mean. */
    mean_.conservativeResize(dim, NoChange);
    mean_.bottomRows(dim_noise) = MatrixXd::Zero(dim_noise, components);

    /* Resize covariance matrix. */
    covariance_.conservativeResizeLike(MatrixXd::Zero(dim_covariance, dim_covariance * components));

    /* Move old covariance matrices from right to left to avoid aliasing.

     Note that the covariance matrix of the 0-th coomponent,
     i.e. in the top-left corner of the matrix covariance_,
     is already in the correct place.
    */
    std::size_t dim_old = use_quaternion ? dim_linear + dim_circular * (dim_circular_component - 1) : dim_linear + dim_circular;
    for (std::size_t i = 0; i < (components - 1); i++)
    {
        std::size_t i_index = components - 1 - i;

        Ref<MatrixXd> new_block = covariance_.block(0, i_index * dim_covariance, dim_old, dim_old);
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
        /* Copy the noise covariance matrix in the bottom-right block of each covariance matrix. */
        covariance_.block(dim_old, i * dim_covariance + dim_old, dim_noise, dim_noise) = noise_covariance_matrix;

        /* Clean part of the matrix that should be zero. */
        covariance_.block(0, i * dim_covariance + dim_old, dim_old, dim_noise) = MatrixXd::Zero(dim_old, dim_noise);

        /* The part in covariance_.block(dim_old, i * dim_covariance, dim_noise, dim_old) was set to 0 when doing
           covariance_.conservativeResizeLike(MatrixXd::Zero(dim_covariance, dim_covariance * components));
           since it is appended in order to expand the matrix. */
    }

    return true;
}
