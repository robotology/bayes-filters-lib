/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef LTIMEMEASUREMENTMODEL_H
#define LTIMEMEASUREMENTMODEL_H

#include <BayesFilters/LinearMeasurementModel.h>

#include <Eigen/Dense>

namespace bfl {
    class LTIMeasurementModel;
}


/**
 * This class represent an linear measurement model Hx + w, where H is a time-invariant measurement matrix, x is a state vector and w is random noise.
 */
class bfl::LTIMeasurementModel : public bfl::LinearMeasurementModel
{
public:
    LTIMeasurementModel(const Eigen::Ref<const Eigen::MatrixXd>& measurement_matrix, const Eigen::Ref<const Eigen::MatrixXd>& noise_covariance_matrix);

    virtual ~LTIMeasurementModel() noexcept = default;

    std::pair<bool, Eigen::MatrixXd> getNoiseCovarianceMatrix() const override;

    Eigen::MatrixXd getMeasurementMatrix() const override;


protected:
    /**
     * Measurement matrix.
     */
    Eigen::MatrixXd H_;

    /**
     * Convariance matrix of the additive white noise of the measurements.
     */
    Eigen::MatrixXd R_;
};

#endif /* LTIMEMEASUREMENTMODEL_H */
