#ifndef VISUALPARTICLEFILTER_H
#define VISUALPARTICLEFILTER_H

#include "FilteringAlgorithm.h"
#include "Initialization.h"
#include "PFVisualCorrection.h"
#include "PFPrediction.h"
#include "Resampling.h"

#include <memory>

namespace bfl{
    class VisualParticleFilter;
}


class bfl::VisualParticleFilter : public FilteringAlgorithm
{
public:
    void setInitialization(std::unique_ptr<Initialization> prediction);

    void setPrediction(std::unique_ptr<PFPrediction> prediction);

    void setCorrection(std::unique_ptr<PFVisualCorrection> correction);

    void setResampling(std::unique_ptr<Resampling> resampling);

    virtual bool skip(const std::string& what_step, const bool status) override;

protected:
    VisualParticleFilter() noexcept;

    VisualParticleFilter(VisualParticleFilter&& pf) noexcept;

    VisualParticleFilter& operator=(VisualParticleFilter&& pf) noexcept;

    std::unique_ptr<Initialization>     initialization_;
    std::unique_ptr<PFPrediction>       prediction_;
    std::unique_ptr<PFVisualCorrection> correction_;
    std::unique_ptr<Resampling>         resampling_;
};

#endif /* VISUALPARTICLEFILTER_H */
