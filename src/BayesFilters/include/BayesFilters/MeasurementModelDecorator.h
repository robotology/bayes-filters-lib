/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef OBSERVATIONMODELDECORATOR_H
#define OBSERVATIONMODELDECORATOR_H

#include <BayesFilters/MeasurementModel.h>

#include <memory>

namespace bfl {
    class MeasurementModelDecorator;
}


class bfl::MeasurementModelDecorator : public MeasurementModel
{
public:
    std::pair<bool, bfl::Data> measure(const Data& data = Data()) const override;

    std::pair<bool, bfl::Data> predictedMeasure(const Eigen::Ref<const Eigen::MatrixXd>& cur_states) const override;

    std::pair<bool, bfl::Data> innovation(const bfl::Data& predicted_measurements, const bfl::Data& measurements) const override;

    std::pair<bool, Eigen::MatrixXd> getNoiseCovarianceMatrix() const override;

    bool setProperty(const std::string& property) override;

    bool freeze(const Data& data = Data()) override;

    std::pair<std::size_t, std::size_t> getOutputSize() const override;

protected:
    MeasurementModelDecorator(std::unique_ptr<MeasurementModel> measurement_model) noexcept;

    MeasurementModelDecorator(MeasurementModelDecorator&& measurement_model) noexcept;

    virtual ~MeasurementModelDecorator() noexcept;

    MeasurementModelDecorator& operator=(MeasurementModelDecorator&& measurement_model) noexcept;

private:
    std::unique_ptr<MeasurementModel> measurement_model;
};

#endif /* OBSERVATIONMODELDECORATOR_H */
