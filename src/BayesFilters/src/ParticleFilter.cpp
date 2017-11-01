#include "BayesFilters/ParticleFilter.h"

using namespace bfl;


ParticleFilter::ParticleFilter() noexcept { }


ParticleFilter::ParticleFilter(ParticleFilter&& pf) noexcept :
    initialization_(std::move(pf.initialization_)),
    prediction_(std::move(pf.prediction_)),
    correction_(std::move(pf.correction_)),
    resampling_(std::move(pf.resampling_)) { }


ParticleFilter& ParticleFilter::operator=(ParticleFilter&& pf) noexcept
{
    initialization_ = std::move(pf.initialization_);
    prediction_     = std::move(pf.prediction_);
    correction_     = std::move(pf.correction_);
    resampling_     = std::move(pf.resampling_);

    return *this;
}


void ParticleFilter::setInitialization(std::unique_ptr<Initialization> initialization)
{
    initialization_ = std::move(initialization);
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


bool ParticleFilter::skip(const std::string& what_step, const bool status)
{
    if (what_step == "prediction")
        return prediction_->skip(what_step, status);

    if (what_step == "correction")
        return correction_->skip(status);

    return false;
}
