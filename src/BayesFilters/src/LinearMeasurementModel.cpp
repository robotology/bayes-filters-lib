/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/LinearMeasurementModel.h>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


std::pair<bool, bfl::Data> LinearMeasurementModel::predictedMeasure(const Eigen::Ref<const Eigen::MatrixXd>& cur_states) const
{
    MatrixXd prediction = getMeasurementMatrix() * cur_states;
    return std::make_pair(true, std::move(prediction));
}


std::pair<bool, bfl::Data> LinearMeasurementModel::innovation(const bfl::Data& predicted_measurements, const bfl::Data& measurements) const
{
    MatrixXd innovation = -(any::any_cast<MatrixXd>(predicted_measurements).colwise() - any::any_cast<MatrixXd>(measurements).col(0));

    return std::make_pair(true, std::move(innovation));
}
