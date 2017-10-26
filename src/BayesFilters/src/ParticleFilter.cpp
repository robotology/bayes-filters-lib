#include "BayesFilters/ParticleFilter.h"

using namespace bfl;


ParticleFilter::ParticleFilter() noexcept { }


ParticleFilter::ParticleFilter(ParticleFilter&& pf) noexcept :
    prediction_(std::move(pf.prediction_)), correction_(std::move(pf.correction_)), resampling_(std::move(pf.resampling_)) { }


ParticleFilter& ParticleFilter::operator=(ParticleFilter&& pf) noexcept
{
    prediction_ = std::move(pf.prediction_);
    correction_ = std::move(pf.correction_);
    resampling_ = std::move(pf.resampling_);

    return *this;
}


void ParticleFilter::setPrediction(std::unique_ptr<PFPrediction> prediction)
{
    prediction_ = std::move(prediction);
}


void ParticleFilter::setCorrection(std::unique_ptr<PFCorrection> correction)
{
    correction_ = std::move(correction);
}


void ParticleFilter::setResampling(std::unique_ptr<Resampling> resampling)
{
    resampling_ = std::move(resampling);
}


void ParticleFilter::skip(const std::string& what_step, const bool status)
{
    if (what_step == "prediction")
        prediction_->skip(status);

    if (what_step == "correction")
        correction_->skip(status);
}
