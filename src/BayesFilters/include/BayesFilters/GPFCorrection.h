#ifndef GPFCORRECTION_H
#define GPFCORRECTION_H

#include <BayesFilters/GaussianCorrection.h>
#include <BayesFilters/LikelihoodModel.h>
#include <BayesFilters/MeasurementModel.h>
#include <BayesFilters/ParticleSet.h>
#include <BayesFilters/PFCorrection.h>
#include <BayesFilters/StateModel.h>

#include <functional>
#include <memory>
#include <random>

namespace bfl {
    class GPFCorrection;
}


class bfl::GPFCorrection : public bfl::PFCorrection
{
public:
    GPFCorrection(std::unique_ptr<bfl::GaussianCorrection> gaussian_correction, std::unique_ptr<bfl::LikelihoodModel> likihood_model, std::unique_ptr<bfl::StateModel> state_model) noexcept;

    GPFCorrection(std::unique_ptr<bfl::GaussianCorrection> gaussian_correction, std::unique_ptr<bfl::LikelihoodModel> likelihood_model, std::unique_ptr<bfl::StateModel> state_model, unsigned int seed) noexcept;

    GPFCorrection(GPFCorrection&& gpf_correction) noexcept;

    virtual ~GPFCorrection() noexcept { };

    void setLikelihoodModel(std::unique_ptr<bfl::LikelihoodModel> likelihood_model) override;

    void setMeasurementModel(std::unique_ptr<bfl::MeasurementModel> measurement_model) override;

    bfl::MeasurementModel& getMeasurementModel() override;

    bfl::LikelihoodModel& getLikelihoodModel() override;

    std::pair<bool, Eigen::VectorXd> getLikelihood() override;

protected:
    void correctStep(const bfl::ParticleSet& pred_particles, bfl::ParticleSet& corr_particles) override;

    Eigen::VectorXd sampleFromProposal(const Eigen::VectorXd& mean, const Eigen::MatrixXd& covariance);

    double evaluateProposal(const Eigen::VectorXd& state, const Eigen::VectorXd& mean, const Eigen::MatrixXd& covariance);

    std::unique_ptr<bfl::GaussianCorrection> gaussian_correction_;

    std::unique_ptr<bfl::LikelihoodModel> likelihood_model_;

    /**
     * The state model is required to evaluate the Markov transition probability
     * that is used in the update of the particles weight.
     */
    std::unique_ptr<bfl::StateModel> state_model_;

    std::mt19937_64 generator_;

    std::normal_distribution<double> distribution_;

    /**
     * Random number generator function from a Normal distribution.
     * A call to `gaussian_random_sample_()` returns a double-precision floating-point random number.
     */
    std::function<double()> gaussian_random_sample_;
};

#endif /* GPFCORRECTION_H */
