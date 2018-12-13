#ifndef GAUSSIANFILTER_H
#define GAUSSIANFILTER_H

#include <BayesFilters/FilteringAlgorithm.h>
#include <BayesFilters/Gaussian.h>
#include <BayesFilters/GaussianPrediction.h>
#include <BayesFilters/GaussianCorrection.h>

namespace bfl {
    class GaussianFilter;
}


class bfl::GaussianFilter: public bfl::FilteringAlgorithm
{
public:
    GaussianFilter(Gaussian& initial_state, std::unique_ptr<GaussianPrediction> prediction, std::unique_ptr<GaussianCorrection> correction) noexcept;

    GaussianFilter(GaussianFilter&& kf) noexcept;

    GaussianFilter& operator=(GaussianFilter&& gf) noexcept;

    virtual ~GaussianFilter() noexcept;

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

#endif /* GAUSSIANFILTER_H */
