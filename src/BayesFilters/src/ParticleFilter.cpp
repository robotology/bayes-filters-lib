/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/ParticleFilter.h>

using namespace bfl;


ParticleFilter::ParticleFilter
(
    std::unique_ptr<ParticleSetInitialization> initialization,
    std::unique_ptr<PFPrediction> prediction,
    std::unique_ptr<PFCorrection> correction,
    std::unique_ptr<Resampling> resampling
)
noexcept :
    initialization_(std::move(initialization)),
    prediction_(std::move(prediction)),
    correction_(std::move(correction)),
    resampling_(std::move(resampling))
{ }


bool ParticleFilter::skip(const std::string& what_step, const bool status)
{
    if (what_step == "prediction" ||
        what_step == "state"      ||
        what_step == "exogenous"    )
        return prediction_->skip(what_step, status);

    if (what_step == "correction")
        return correction_->skip(status);

    if (what_step == "all")
    {
        bool return_status = true;

        return_status &= prediction_->skip("prediction", status);

        return_status &= correction_->skip(status);

        return return_status;
    }

    return false;
}


ParticleSetInitialization& ParticleFilter::initialization()
{
    return *initialization_;
}


PFPrediction& ParticleFilter::prediction()
{
    return *prediction_;
}


PFCorrection& ParticleFilter::correction()
{
    return *correction_;
}


Resampling& ParticleFilter::resampling()
{
    return *resampling_;
}
