#ifndef PARTICLEFILTERCORRECTION_H
#define PARTICLEFILTERCORRECTION_H

#include <memory>
#include <random>

#include "Correction.h"
#include "ObservationModel.h"


class ParticleFilterCorrection: public Correction
{
public:
    /* Default constructor, disabled */
    ParticleFilterCorrection() = delete;

    /* PF correction constructor */
    ParticleFilterCorrection(std::shared_ptr<ObservationModel> observation_model) noexcept;

    /* Destructor */
    ~ParticleFilterCorrection() noexcept override;

    /* Copy constructor */
    ParticleFilterCorrection(const ParticleFilterCorrection& pf_correction);

    /* Move constructor */
    ParticleFilterCorrection(ParticleFilterCorrection&& pf_correction) noexcept;

    /* Copy assignment operator */
    ParticleFilterCorrection& operator=(const ParticleFilterCorrection& pf_correction);

    /* Move assignment operator */
    ParticleFilterCorrection& operator=(ParticleFilterCorrection&& pf_correction) noexcept;

    void correct(const Eigen::Ref<const Eigen::VectorXf>& pred_state, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::VectorXf> cor_state) override;

protected:
    std::shared_ptr<ObservationModel> measurement_model_;
};

#endif /* PARTICLEFILTERCORRECTION_H */
