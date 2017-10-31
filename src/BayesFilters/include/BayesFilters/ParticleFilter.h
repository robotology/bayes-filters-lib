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
    void setPrediction(std::unique_ptr<PFPrediction> prediction);

    void setCorrection(std::unique_ptr<PFCorrection> correction);

    void setResampling(std::unique_ptr<Resampling> resampling);

    virtual bool skip(const std::string& what_step, const bool status) override;

protected:
    ParticleFilter() noexcept;

    ParticleFilter(ParticleFilter&& pf) noexcept;

    ParticleFilter& operator=(ParticleFilter&& pf) noexcept;

    std::unique_ptr<PFPrediction> prediction_;
    std::unique_ptr<PFCorrection> correction_;
    std::unique_ptr<Resampling>   resampling_;
};

#endif /* PARTICLEFILTER_H */
