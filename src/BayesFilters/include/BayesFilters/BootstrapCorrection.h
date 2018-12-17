#ifndef UPDATEPARTICLES_H
#define UPDATEPARTICLES_H

#include <BayesFilters/ParticleSet.h>
#include <BayesFilters/PFCorrection.h>

#include <memory>
#include <random>

namespace bfl {
    class BoostrapCorrection;
}


class bfl::BoostrapCorrection : public PFCorrection
{
public:
    BoostrapCorrection() noexcept;

    virtual ~BoostrapCorrection() noexcept;

    void setLikelihoodModel(std::unique_ptr<LikelihoodModel> likelihood_model) override;

    void setMeasurementModel(std::unique_ptr<MeasurementModel> measurement_model) override;

    std::pair<bool, Eigen::VectorXd> getLikelihood() override;

protected:
    std::unique_ptr<LikelihoodModel> likelihood_model_;

    std::unique_ptr<MeasurementModel> measurement_model_;

    LikelihoodModel& getLikelihoodModel() override;

    MeasurementModel& getMeasurementModel() override;

    void correctStep(const ParticleSet& pred_particles, ParticleSet& cor_particles) override;

    bool valid_likelihood_ = false;
    Eigen::VectorXd likelihood_;
};

#endif /* UPDATEPARTICLES_H */
