#ifndef LTIMEMEASUREMENTMODEL_H
#define LTIMEMEASUREMENTMODEL_H

#include <BayesFilters/LinearMeasurementModel.h>

#include <Eigen/Dense>

namespace bfl {
    class LTIMeasurementModel;
}


class bfl::LTIMeasurementModel : public bfl::LinearMeasurementModel
{
public:
    LTIMeasurementModel(const Eigen::MatrixXf& measurement_matrix, const Eigen::MatrixXf& noise_covariance_matrix);

    virtual ~LTIMeasurementModel() noexcept { };

    std::pair<bool, Eigen::MatrixXf> getNoiseCovarianceMatrix() const override;

    Eigen::MatrixXf getMeasurementMatrix() const override;

protected:
    /* Measurement matrix. */
    Eigen::MatrixXf H_;

    /* Matrix covariance of the zero mean additive white measurement noise. */
    Eigen::MatrixXf R_;
};

#endif /* LTIMEMEASUREMENTMODEL_H */
