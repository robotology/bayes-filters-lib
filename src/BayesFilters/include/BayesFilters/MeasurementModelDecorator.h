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
    std::pair<bool, bfl::Data> measure() const override;

    std::pair<bool, bfl::Data> predictedMeasure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) const override;

    std::pair<bool, bfl::Data> innovation(const bfl::Data& predicted_measurements, const bfl::Data& measurements) const override;

    std::pair<bool, Eigen::MatrixXf> getNoiseCovarianceMatrix() const override;

    bool setProperty(const std::string& property) override;

    bool bufferAgentData() const override;

protected:
    MeasurementModelDecorator(std::unique_ptr<MeasurementModel> measurement_model) noexcept;

    MeasurementModelDecorator(MeasurementModelDecorator&& measurement_model) noexcept;

    virtual ~MeasurementModelDecorator() noexcept;

    MeasurementModelDecorator& operator=(MeasurementModelDecorator&& measurement_model) noexcept;

private:
    std::unique_ptr<MeasurementModel> measurement_model;
};

#endif /* OBSERVATIONMODELDECORATOR_H */
