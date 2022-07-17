/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef STATEPROCESS_H
#define STATEPROCESS_H

#include <BayesFilters/VectorDescription.h>

#include <Eigen/Dense>

namespace bfl {
    class StateProcess;
}


class bfl::StateProcess
{
public:
    virtual void propagate(const Eigen::Ref<const Eigen::MatrixXd>& cur_states, Eigen::Ref<Eigen::MatrixXd> prop_states) = 0;

    virtual void motion(const Eigen::Ref<const Eigen::MatrixXd>& cur_states, Eigen::Ref<Eigen::MatrixXd> mot_states) = 0;

    virtual bool setProperty(const std::string& property) = 0;

    /**
     * Returns the vector description of the input to the state equation.
     */
    virtual VectorDescription getInputDescription() = 0;

    /**
     * Returns the vector description of the output of the state equation.
     */
    virtual VectorDescription getStateDescription() = 0;
};

#endif /* STATEPROCESS_H */
