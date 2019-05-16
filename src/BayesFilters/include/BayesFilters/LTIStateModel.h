/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef LTISTATEMODEL_H
#define LTISTATEMODEL_H

#include <BayesFilters/LinearStateModel.h>

namespace bfl {
    class LTIStateModel;
}


class bfl::LTIStateModel : public bfl::LinearStateModel
{
public:
    LTIStateModel(const Eigen::Ref<const Eigen::MatrixXd>& transition_matrix, const Eigen::Ref<const Eigen::MatrixXd>& noise_covariance_matrix);

    LTIStateModel(const LTIStateModel& state_model) noexcept = delete;

    LTIStateModel& operator=(const LTIStateModel& state_model) noexcept = delete;

    LTIStateModel(LTIStateModel&& state_model) noexcept;

    LTIStateModel& operator=(LTIStateModel&& state_model) noexcept;

    virtual ~LTIStateModel() noexcept = default;

    bool setProperty(const std::string& property) override;

    Eigen::MatrixXd getJacobian() override;

    Eigen::MatrixXd getNoiseCovarianceMatrix() override;

    Eigen::MatrixXd getStateTransitionMatrix() override;

private:
    /**
     * State transition matrix.
     */
    Eigen::MatrixXd F_;

    /**
     * Noise covariance matrix of a zero mean additive white noise.
     */
    Eigen::MatrixXd Q_;
};

#endif /* LTISTATEMODEL_H */
