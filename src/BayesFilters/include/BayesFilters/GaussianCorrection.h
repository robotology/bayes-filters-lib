#ifndef GAUSSIANCORRECTION_H
#define GAUSSIANCORRECTION_H

#include <BayesFilters/Gaussian.h>
#include <BayesFilters/MeasurementModel.h>

#include <Eigen/Dense>

namespace bfl {
    class GaussianCorrection;
}


class bfl::GaussianCorrection
{
public:
    virtual ~GaussianCorrection() noexcept { };

    Gaussian correct(const Gaussian& pred_state);

    bool skip(const bool status);

    virtual std::pair<bool, Eigen::VectorXd> getLikelihood();

protected:
    virtual Gaussian correctStep(const Gaussian& prev_state) = 0;

    virtual std::pair<bool, Eigen::VectorXd> likelihood(const Eigen::Ref<const Eigen::MatrixXd>& innovations) = 0;

    virtual MeasurementModel& getMeasurementModel() = 0;

    GaussianCorrection() noexcept;

    GaussianCorrection(GaussianCorrection&& gaussian_correction) noexcept;

private:
    bool skip_ = false;

    friend class GaussianCorrectionDecorator;
};

#endif /* GAUSSIANCORRECTION_H */
