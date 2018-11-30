#ifndef LINEARMEMEASUREMENTMODEL_H
#define LINEARMEMEASUREMENTMODEL_H

#include <BayesFilters/MeasurementModel.h>

#include <Eigen/Dense>

namespace bfl {
    class LinearMeasurementModel;
}


class bfl::LinearMeasurementModel : public MeasurementModel
{
public:
    virtual ~LinearMeasurementModel() noexcept { };

    virtual Eigen::MatrixXf getMeasurementMatrix() const = 0;

    virtual std::pair<bool, bfl::Data> predictedMeasure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) const override;

    virtual std::pair<bool, bfl::Data> innovation(const bfl::Data& predicted_measurements, const bfl::Data& measurements) const override;
};

#endif /* LINEARMEMEASUREMENTMODEL_H */
