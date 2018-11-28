#ifndef UPDATEPARTICLES_H
#define UPDATEPARTICLES_H

#include <BayesFilters/ParticleSet.h>
#include <BayesFilters/PFCorrection.h>

#include <memory>
#include <random>

namespace bfl {
    class UpdateParticles;
}


class bfl::UpdateParticles : public PFCorrection
{
public:
    UpdateParticles() noexcept;

    virtual ~UpdateParticles() noexcept;

    void setLikelihoodModel(std::unique_ptr<LikelihoodModel> likelihood_model) override;

    void setMeasurementModel(std::unique_ptr<MeasurementModel> measurement_model) override;

    std::pair<bool, Eigen::VectorXf> getLikelihood() override;

protected:
    std::unique_ptr<LikelihoodModel> likelihood_model_;

    std::unique_ptr<MeasurementModel> measurement_model_;

    LikelihoodModel& getLikelihoodModel() override;

    MeasurementModel& getMeasurementModel() override;

    void correctStep(const ParticleSet& pred_particles, ParticleSet& cor_particles) override;

    bool valid_likelihood_ = false;
    Eigen::VectorXf likelihood_;
};

#endif /* UPDATEPARTICLES_H */
