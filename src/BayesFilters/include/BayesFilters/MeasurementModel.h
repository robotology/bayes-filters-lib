#ifndef MEASUREMENTMODEL_H
#define MEASUREMENTMODEL_H

#include <string>
#include <utility>

#include <Eigen/Dense>

namespace bfl {
    class MeasurementModel;
}


class bfl::MeasurementModel
{
public:
    virtual ~MeasurementModel() noexcept { };

    virtual std::pair<bool, Eigen::MatrixXf> measure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) = 0;

    virtual std::pair<bool, Eigen::MatrixXf> innovation(const Eigen::Ref<const Eigen::MatrixXf>& predicted_measurements, const Eigen::Ref<const Eigen::MatrixXf>& measurements) = 0;

    virtual std::pair<bool, Eigen::MatrixXf> getProcessMeasurements();

    virtual std::pair<bool, Eigen::MatrixXf> predictedMeasure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states);

    virtual std::pair<bool, Eigen::MatrixXf> getNoiseSample(const int num);

    virtual std::pair<bool, Eigen::MatrixXf> getNoiseCovarianceMatrix();

    virtual bool setProperty(const std::string property);
};

#endif /* MEASUREMENTMODEL_H */
