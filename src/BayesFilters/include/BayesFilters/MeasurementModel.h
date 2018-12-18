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

    virtual std::pair<bool, bfl::Data> measure() const = 0;

    virtual std::pair<bool, bfl::Data> predictedMeasure(const Eigen::Ref<const Eigen::MatrixXd>& cur_states) const = 0;

    virtual std::pair<bool, bfl::Data> innovation(const bfl::Data& predicted_measurements, const bfl::Data& measurements) const = 0;

    virtual std::pair<bool, Eigen::MatrixXd> getNoiseCovarianceMatrix() const;

    virtual bool setProperty(const std::string& property);

    virtual bool freezeMeasurements() = 0;

    /* Returns the linear and circular size of the output of the measurement equation. */
    virtual std::pair<std::size_t, std::size_t> getOutputSize() const = 0;
};

#endif /* MEASUREMENTMODEL_H */
