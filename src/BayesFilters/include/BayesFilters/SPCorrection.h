#ifndef SPCORRECTION_H
#define SPCORRECTION_H

#include <BayesFilters/Gaussian.h>
#include <BayesFilters/MeasurementModel.h>

#include <memory>
#include <utility>

#include <Eigen/Dense>

namespace bfl {
    class SPCorrection;
}


class bfl::SPCorrection
{
public:
    virtual ~SPCorrection() noexcept { };

    Gaussian correct(const Gaussian& pred_state);

    bool skip(const bool status);

    /* FIXME
       While setObservationModel() will be kept in future implementation, measurement_model_ member is
       currently set with public visibility for backward compatibility. It will be moved to private in future releases. */
    void setMeasurementModel(std::unique_ptr<MeasurementModel> observation_model);

    std::unique_ptr<MeasurementModel> measurement_model_;

    virtual std::pair<bool, Eigen::VectorXd> getLikelihood();

protected:
    virtual Gaussian correctStep(const Gaussian& prev_state) = 0;

    virtual std::pair<bool, Eigen::VectorXd> likelihood(const Eigen::Ref<const Eigen::MatrixXd>& innovations) = 0;

    SPCorrection() noexcept;

    SPCorrection(SPCorrection&& sp_correction) noexcept;

private:
    bool skip_ = false;

    friend class SPCorrectionDecorator;
};

#endif /* SPCORRECTION_H */
