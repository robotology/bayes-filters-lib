/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef VECTORDESCRIPTION_H
#define VECTORDESCRIPTION_H

#include <cstddef>

namespace bfl {
    class VectorDescription;
}


class bfl::VectorDescription
{
public:
    enum class CircularType { Euler, Quaternion };

    VectorDescription() = default;

    VectorDescription(const std::size_t linear_components, const std::size_t circular_components = 0, const std::size_t noise_components = 0, const CircularType& circular_type = CircularType::Euler);

    ~VectorDescription() = default;

    std::size_t linear_components() const;

    std::size_t circular_components() const;

    std::size_t noise_components() const;

    std::size_t linear_size() const;

    std::size_t circular_size() const;

    std::size_t noise_size() const;

    std::size_t total_size() const;

    std::size_t dof_size() const;

    void add_noise_components(const std::size_t& components);

    VectorDescription noiseless_description() const;

    CircularType circular_type = CircularType::Euler;

private:

    std::size_t linear_components_ = 0;

    std::size_t circular_components_ = 0;

    std::size_t noise_components_ = 0;
};

#endif /* VECTORDESCRIPTION_H */
