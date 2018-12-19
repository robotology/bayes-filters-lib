#ifndef KFCORRECTION_H
#define KFCORRECTION_H

#include <BayesFilters/GaussianCorrection.h>
#include <BayesFilters/GaussianMixture.h>
#include <BayesFilters/LinearMeasurementModel.h>

#include <Eigen/Dense>

namespace bfl {
    class KFCorrection;
}


class bfl::KFCorrection : public bfl::GaussianCorrection
{
public:
    KFCorrection(std::unique_ptr<LinearMeasurementModel> measurement_model) noexcept;

    KFCorrection(KFCorrection&& kf_prediction) noexcept;

    virtual ~KFCorrection() noexcept;

    MeasurementModel& getMeasurementModel() override;

protected:
    void correctStep(const GaussianMixture& pred_state, GaussianMixture& corr_state) override;

    std::pair<bool, Eigen::VectorXd> likelihood(const Eigen::Ref<const Eigen::MatrixXd>& innovations) override;

private:
    std::unique_ptr<LinearMeasurementModel> measurement_model_;
};

#endif /* KFCORRECTION_H */
