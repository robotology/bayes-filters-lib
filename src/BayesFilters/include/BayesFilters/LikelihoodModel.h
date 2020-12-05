/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef LIKELIHOODMODEL_H
#define LIKELIHOODMODEL_H

#include <BayesFilters/MeasurementModel.h>

#include <Eigen/Core>

namespace bfl {
    class LikelihoodModel;
}


class bfl::LikelihoodModel
{
public:
    virtual ~LikelihoodModel() noexcept = default;

    virtual std::pair<bool, Eigen::VectorXd> likelihood(const MeasurementModel& measurement_model, const Eigen::Ref<const Eigen::MatrixXd>& pred_states) = 0;
};

#endif /* LIKELIHOODMODEL_H */
