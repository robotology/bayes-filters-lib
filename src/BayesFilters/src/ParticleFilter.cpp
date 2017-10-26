#include "BayesFilters/ParticleFilter.h"

using namespace bfl;


void ParticleFilter::skip(const std::string& what_step, const bool status)
{
    if (what_step == "prediction")
        prediction_->skip(status);
    if (what_step == "correction");

}
