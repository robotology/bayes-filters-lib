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
    virtual ~MeasurementModel() noexcept;

    virtual std::pair<bool, bfl::Data> measure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) const = 0;

    virtual std::pair<bool, bfl::Data> predictedMeasure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) const = 0;

    virtual std::pair<bool, bfl::Data> innovation(const bfl::Data& predicted_measurements, const bfl::Data& measurements) const = 0;

    virtual std::pair<bool, Eigen::MatrixXf> getNoiseCovarianceMatrix() const;

    virtual bool setProperty(const std::string& property);

    virtual bool bufferAgentData() const = 0;

    virtual std::pair<bool, bfl::Data> getAgentMeasurements() const = 0;
};

#endif /* MEASUREMENTMODEL_H */
