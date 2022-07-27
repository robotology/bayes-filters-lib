/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/VectorDescription.h>

using namespace bfl;


VectorDescription::VectorDescription
(
    const std::size_t linear_components,
    const std::size_t circular_components,
    const std::size_t noise_components,
    const VectorDescription::CircularType& circular_type
) :
    linear_components_(linear_components),
    circular_components_(circular_components),
    noise_components_(noise_components),
    circular_type(circular_type)
{}


std::size_t VectorDescription::linear_components() const
{
    return linear_components_;
}


std::size_t VectorDescription::circular_components() const
{
    return circular_components_;
}


std::size_t VectorDescription::noise_components() const
{
    return noise_components_;
}


std::size_t VectorDescription::linear_size() const
{
    return linear_components_;
}


std::size_t VectorDescription::circular_size() const
{
    if (circular_type == CircularType::Quaternion)
        return circular_components_ * 4;

    return circular_components_;
}


std::size_t VectorDescription::noise_size() const
{
    return noise_components_;
}


std::size_t VectorDescription::total_size() const
{
    return linear_size() + circular_size() + noise_size();
}


std::size_t VectorDescription::dof_size() const
{
    if (circular_type == CircularType::Quaternion)
    {
        /* If quaternions are used, the number of degrees of freedom for the evaluation of the sigma points
           depends on the dimensions of the tangent space that is R^3. */
        return linear_size() + circular_components() * 3 + noise_size();
    }
    else
        return total_size();
}


void VectorDescription::add_noise_components(const std::size_t& components)
{
    noise_components_ += components;
}


VectorDescription VectorDescription::noiseless_description() const
{
    return VectorDescription(linear_components_, circular_components_, 0, circular_type);
}
