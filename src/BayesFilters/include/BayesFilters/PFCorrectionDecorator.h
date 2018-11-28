#ifndef PFCORRECTIONDECORATOR_H
#define PFCORRECTIONDECORATOR_H

#include <BayesFilters/ParticleSet.h>
#include <BayesFilters/PFCorrection.h>

#include <memory>

namespace bfl {
    class PFCorrectionDecorator;
}


class bfl::PFCorrectionDecorator : public PFCorrection
{
public:
    void setLikelihoodModel(std::unique_ptr<LikelihoodModel> observation_model) override;

    void setMeasurementModel(std::unique_ptr<MeasurementModel> measurement_model) override;

    std::pair<bool, Eigen::VectorXf> getLikelihood() override;

protected:
    PFCorrectionDecorator(std::unique_ptr<PFCorrection> correction) noexcept;

    PFCorrectionDecorator(PFCorrectionDecorator&& correction) noexcept;

    virtual ~PFCorrectionDecorator() noexcept;

    LikelihoodModel& getLikelihoodModel() override;

    MeasurementModel& getMeasurementModel() override;

    void correctStep(const bfl::ParticleSet& pred_particles, bfl::ParticleSet& cor_particles) override;

private:
    std::unique_ptr<PFCorrection> correction_;
};

#endif /* PFCORRECTIONDECORATOR_H */
