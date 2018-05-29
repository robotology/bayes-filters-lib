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
    std::pair<bool, Eigen::MatrixXf> measure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) override;

    std::pair<bool, Eigen::MatrixXf> getProcessMeasurements() override;

    std::pair<bool, Eigen::MatrixXf> innovation(const Eigen::Ref<const Eigen::MatrixXf>& predicted_measurements, const Eigen::Ref<const Eigen::MatrixXf>& measurements) override;

    std::pair<bool, Eigen::MatrixXf> predictedMeasure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) override;

    std::pair<bool, Eigen::MatrixXf> getNoiseSample(const int num) override;

    std::pair<bool, Eigen::MatrixXf> getNoiseCovarianceMatrix() override;

    bool setProperty(const std::string property) override;

protected:
    MeasurementModelDecorator(std::unique_ptr<MeasurementModel> measurement_model) noexcept;

    MeasurementModelDecorator(MeasurementModelDecorator&& measurement_model) noexcept;

    virtual ~MeasurementModelDecorator() noexcept;

    MeasurementModelDecorator& operator=(MeasurementModelDecorator&& measurement_model) noexcept;

private:
    std::unique_ptr<MeasurementModel> measurement_model;
};

#endif /* OBSERVATIONMODELDECORATOR_H */
