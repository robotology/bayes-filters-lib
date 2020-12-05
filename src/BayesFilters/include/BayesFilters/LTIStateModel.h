/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef LTISTATEMODEL_H
#define LTISTATEMODEL_H

#include <Eigen/Dense>

#include <BayesFilters/LinearStateModel.h>

namespace bfl {
    class LTIStateModel;
}


class bfl::LTIStateModel : public bfl::LinearStateModel
{
public:
    LTIStateModel(const Eigen::Ref<const Eigen::MatrixXd>& transition_matrix, const Eigen::Ref<const Eigen::MatrixXd>& noise_covariance_matrix);

    virtual ~LTIStateModel() noexcept = default;

    void propagate(const Eigen::Ref<const Eigen::MatrixXd>& cur_states, Eigen::Ref<Eigen::MatrixXd> prop_states) override;

    Eigen::MatrixXd getNoiseCovarianceMatrix() override;

    Eigen::MatrixXd getStateTransitionMatrix() override;

    bool setProperty(const std::string& property) override;

    Eigen::MatrixXd getJacobian() override;

protected:
    /*
     * State transition matrix.
     */
    Eigen::MatrixXd F_;

    /*
     * Noise covariance matrix of zero mean additive white noise.
     */
    Eigen::MatrixXd Q_;
};

#endif /* LTISTATEMODEL_H */
