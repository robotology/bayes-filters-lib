#ifndef MEASUREMENTMODEL_H
#define MEASUREMENTMODEL_H

#include <BayesFilters/Data.h>

#include <memory>
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

    virtual std::pair<bool, bfl::Data> measure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) const = 0;

    virtual std::pair<bool, bfl::Data> predictedMeasure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) const = 0;

    virtual std::pair<bool, bfl::Data> innovation(const bfl::Data& predicted_measurements, const bfl::Data& measurements) const = 0;

    virtual std::pair<bool, Eigen::MatrixXf> getNoiseSample(const int num) const;

    virtual std::pair<bool, Eigen::MatrixXf> getNoiseCovarianceMatrix() const;

    virtual bool setProperty(const std::string& property);

    virtual std::pair<bool, bfl::Data> getProcessMeasurements(const bfl::Data& process_data) const = 0;

    virtual std::pair<bool, Eigen::MatrixXf> getProcessMeasurements() const = 0;

    virtual void enableLog(const std::string& prefix_name);

    virtual void disableLog();
};

#endif /* MEASUREMENTMODEL_H */
