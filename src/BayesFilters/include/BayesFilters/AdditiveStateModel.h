/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef ADDITIVESTATEMODEL_H
#define ADDITIVESTATEMODEL_H

#include <Eigen/Dense>

#include <BayesFilters/StateModel.h>

namespace bfl {
    class AdditiveStateModel;
}


class bfl::AdditiveStateModel : public bfl::StateModel
{
public:
    virtual ~AdditiveStateModel() noexcept = default;

    virtual void motion(const Eigen::Ref<const Eigen::MatrixXd>& cur_states, Eigen::Ref<Eigen::MatrixXd> mot_states) override;
};

#endif /* ADDITIVESTATEMODEL_H */
