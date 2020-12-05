/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef MEASUREMENTMODEL_H
#define MEASUREMENTMODEL_H

#include <BayesFilters/Data.h>
#include <BayesFilters/Logger.h>

#include <memory>
#include <string>
#include <utility>

#include <Eigen/Dense>

namespace bfl {
    class MeasurementModel;
}


class bfl::MeasurementModel : public Logger
{
public:
    virtual ~MeasurementModel() noexcept = default;

    virtual bool freeze(const Data& data = Data()) = 0;

    virtual std::pair<bool, Data> measure(const Data& data = Data()) const = 0;

    virtual std::pair<bool, Data> predictedMeasure(const Eigen::Ref<const Eigen::MatrixXd>& cur_states) const = 0;

    virtual std::pair<bool, Data> innovation(const Data& predicted_measurements, const Data& measurements) const = 0;

    virtual std::pair<bool, Eigen::MatrixXd> getNoiseCovarianceMatrix() const;

    virtual bool setProperty(const std::string& property);

    /* Returns the linear and circular size of the output of the measurement equation. */
    virtual std::pair<std::size_t, std::size_t> getOutputSize() const = 0;
};

#endif /* MEASUREMENTMODEL_H */
