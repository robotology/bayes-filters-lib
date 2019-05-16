/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef LINEARMEMEASUREMENTMODEL_H
#define LINEARMEMEASUREMENTMODEL_H

#include <BayesFilters/AdditiveMeasurementModel.h>

namespace bfl {
    class LinearMeasurementModel;
}


class bfl::LinearMeasurementModel : public bfl::AdditiveMeasurementModel
{
public:
    virtual ~LinearMeasurementModel() noexcept = default;

    virtual Eigen::MatrixXd getMeasurementMatrix() const = 0;

    virtual std::pair<bool, bfl::Data> predictedMeasure(const Eigen::Ref<const Eigen::MatrixXd>& cur_states) const override;

    virtual std::pair<bool, bfl::Data> innovation(const bfl::Data& predicted_measurements, const bfl::Data& measurements) const override;
};

#endif /* LINEARMEMEASUREMENTMODEL_H */
