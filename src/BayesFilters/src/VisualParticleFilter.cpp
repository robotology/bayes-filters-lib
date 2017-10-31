#include "BayesFilters/VisualParticleFilter.h"

using namespace bfl;


VisualParticleFilter::VisualParticleFilter() noexcept { }


VisualParticleFilter::VisualParticleFilter(VisualParticleFilter&& pf) noexcept :
    initialization_(std::move(pf.initialization_)),
    prediction_(std::move(pf.prediction_)),
    correction_(std::move(pf.correction_)),
    resampling_(std::move(pf.resampling_)) { }


VisualParticleFilter& VisualParticleFilter::operator=(VisualParticleFilter&& pf) noexcept
{
    initialization_ = std::move(pf.initialization_);
    prediction_     = std::move(pf.prediction_);
    correction_     = std::move(pf.correction_);
    resampling_     = std::move(pf.resampling_);

    return *this;
}


void VisualParticleFilter::setInitialization(std::unique_ptr<Initialization> initialization)
{
    initialization_ = std::move(initialization);
}


void VisualParticleFilter::setPrediction(std::unique_ptr<PFPrediction> prediction)
{
    prediction_ = std::move(prediction);
}


void VisualParticleFilter::setCorrection(std::unique_ptr<PFVisualCorrection> correction)
{
    correction_ = std::move(correction);
}


void VisualParticleFilter::setResampling(std::unique_ptr<Resampling> resampling)
{
    resampling_ = std::move(resampling);
}


bool VisualParticleFilter::skip(const std::string& what_step, const bool status)
{
    if (what_step == "prediction")
        return prediction_->skip(status);

    if (what_step == "correction")
        return correction_->skip(status);

    return false;
}
