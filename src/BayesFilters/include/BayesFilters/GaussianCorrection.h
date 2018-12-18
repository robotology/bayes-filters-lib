#ifndef GAUSSIANCORRECTION_H
#define GAUSSIANCORRECTION_H

#include <BayesFilters/GaussianMixture.h>
#include <BayesFilters/MeasurementModel.h>

#include <Eigen/Dense>

namespace bfl {
    class GaussianCorrection;
}


class bfl::GaussianCorrection
{
public:
    virtual ~GaussianCorrection() noexcept { };

    void correct(const GaussianMixture& pred_state, GaussianMixture& corr_state);

    bool skip(const bool status);

    virtual std::pair<bool, Eigen::VectorXd> getLikelihood();

protected:
    virtual void correctStep(const GaussianMixture& pred_state, GaussianMixture& corr_state) = 0;

    virtual std::pair<bool, Eigen::VectorXd> likelihood(const Eigen::Ref<const Eigen::MatrixXd>& innovations) = 0;

    virtual MeasurementModel& getMeasurementModel() = 0;

    GaussianCorrection() noexcept;

    GaussianCorrection(GaussianCorrection&& gaussian_correction) noexcept;

private:
    bool skip_ = false;

    friend class GaussianCorrectionDecorator;

    friend class GPFCorrection;
};

#endif /* GAUSSIANCORRECTION_H */
