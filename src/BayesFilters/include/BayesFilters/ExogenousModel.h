/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef EXOGENOUSMODEL_H
#define EXOGENOUSMODEL_H

#include <Eigen/Dense>

namespace bfl {
    class ExogenousModel;
}


class bfl::ExogenousModel
{
public:
    virtual ~ExogenousModel() noexcept = default;

    virtual void propagate(const Eigen::Ref<const Eigen::MatrixXd>& cur_states, Eigen::Ref<Eigen::MatrixXd> prop_states) = 0;

    virtual Eigen::MatrixXd getExogenousMatrix() = 0;

    virtual bool setProperty(const std::string& property) = 0;

    virtual std::pair<std::size_t, std::size_t> getOutputSize() const = 0;
};

#endif /* EXOGENOUSMODEL_H */
