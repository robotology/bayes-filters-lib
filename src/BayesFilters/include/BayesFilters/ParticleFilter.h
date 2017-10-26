#ifndef PARTICLEFILTER_H
#define PARTICLEFILTER_H

#include "FilteringAlgorithm.h"
#include "PFCorrection.h"
#include "PFPrediction.h"
#include "Resampling.h"

#include <memory>

namespace bfl{
    class ParticleFilter;
}


class bfl::ParticleFilter : public FilteringAlgorithm
{
public:
    virtual void setPrediction(std::unique_ptr<PFPrediction> prediction) = 0;

    virtual void setCorrection(std::unique_ptr<PFCorrection> correction) = 0;

    virtual void setResampling(std::unique_ptr<Resampling> resampling) = 0;

    virtual void skip(const std::string& what_step, const bool status) override;

protected:
    std::unique_ptr<PFPrediction> prediction_;
    std::unique_ptr<PFCorrection> correction_;
    std::unique_ptr<Resampling>   resampling_;
};

#endif /* PARTICLEFILTER_H */
