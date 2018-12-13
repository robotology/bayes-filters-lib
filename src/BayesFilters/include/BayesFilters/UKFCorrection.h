#ifndef UKFCORRECTION_H
#define UKFCORRECTION_H

#include <BayesFilters/AdditiveMeasurementModel.h>
#include <BayesFilters/GaussianCorrection.h>
#include <BayesFilters/GaussianMixture.h>
#include <BayesFilters/MeasurementModel.h>
#include <BayesFilters/sigma_point.h>

#include <Eigen/Dense>

namespace bfl {
    class UKFCorrection;
}


class bfl::UKFCorrection : public bfl::GaussianCorrection
{
public:
    UKFCorrection(std::unique_ptr<bfl::MeasurementModel> meas_model, const size_t n, const double alpha, const double beta, const double kappa) noexcept;

    UKFCorrection(std::unique_ptr<bfl::AdditiveMeasurementModel> meas_model, const size_t n, const double alpha, const double beta, const double kappa) noexcept;

    UKFCorrection(UKFCorrection&& ukf_prediction) noexcept;

    virtual ~UKFCorrection() noexcept;

    MeasurementModel& getMeasurementModel() override;

protected:
    void correctStep(const bfl::GaussianMixture& pred_state, bfl::GaussianMixture& corr_state) override;

    std::pair<bool, Eigen::VectorXd> likelihood(const Eigen::Ref<const Eigen::MatrixXd>& innovations) override;

    std::unique_ptr<bfl::MeasurementModel> measurement_model_;

    std::unique_ptr<bfl::AdditiveMeasurementModel> additive_measurement_model_;

    enum class UKFCorrectionType { Generic, Additive };

    /**
     * Distinguish between a UKFCorrection using a generic MeasurmentModel
     * and a UKFCorrection using an AdditiveMeasurementModel.
     */
    UKFCorrectionType type_;

    /**
     * Unscented transform weight.
     */
    bfl::sigma_point::UTWeight ut_weight_;
};

#endif /* UKFCORRECTION_H */
