#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <BayesFilters/FilteringAlgorithm.h>
#include <BayesFilters/Gaussian.h>
#include <BayesFilters/GaussianPrediction.h>
#include <BayesFilters/GaussianCorrection.h>

namespace bfl {
    class KalmanFilter;
}


class bfl::KalmanFilter: public bfl::FilteringAlgorithm
{
public:
    KalmanFilter(Gaussian& initial_state, std::unique_ptr<GaussianPrediction> prediction, std::unique_ptr<GaussianCorrection> correction) noexcept;

    KalmanFilter(KalmanFilter&& kf) noexcept;

    KalmanFilter& operator=(KalmanFilter&& pf) noexcept;

    virtual ~KalmanFilter() noexcept;

    bool initialization() override;

    void filteringStep() override;

    bool runCondition() override;

    bool skip(const std::string& what_step, const bool status) override;

protected:
    Gaussian predicted_state_;

    Gaussian corrected_state_;

    std::unique_ptr<GaussianPrediction> prediction_;

    std::unique_ptr<GaussianCorrection> correction_;
};

#endif /* KALMANFILTER_H */
