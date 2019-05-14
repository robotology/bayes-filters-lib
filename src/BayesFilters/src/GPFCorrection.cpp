/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/GPFCorrection.h>
#include <BayesFilters/utils.h>

#include <Eigen/Cholesky>

#include <exception>

using namespace bfl;
using namespace Eigen;


GPFCorrection::GPFCorrection
(
    std::unique_ptr<LikelihoodModel> likelihood_model,
    std::unique_ptr<GaussianCorrection> gaussian_correction,
    std::unique_ptr<StateModel> state_model
) noexcept :
    GPFCorrection(std::move(likelihood_model), std::move(gaussian_correction), std::move(state_model), 1)
{ }


GPFCorrection::GPFCorrection
(
    std::unique_ptr<LikelihoodModel> likelihood_model,
    std::unique_ptr<GaussianCorrection> gaussian_correction,
    std::unique_ptr<StateModel> state_model,
    unsigned int seed
) noexcept :
    likelihood_model_(std::move(likelihood_model)),
    gaussian_correction_(std::move(gaussian_correction)),
    state_model_(std::move(state_model)),
    generator_(std::mt19937_64(seed)),
    distribution_(std::normal_distribution<double>(0.0, 1.0)),
    gaussian_random_sample_([&] { return (distribution_)(generator_); })
{ }


GPFCorrection::GPFCorrection(GPFCorrection&& correction) noexcept :
    PFCorrection(std::move(correction)),
    likelihood_model_(std::move(correction.likelihood_model_)),
    gaussian_correction_(std::move(correction.gaussian_correction_)),
    state_model_(std::move(correction.state_model_)),
    generator_(std::move(correction.generator_)),
    distribution_(std::move(correction.distribution_)),
    gaussian_random_sample_(std::move(correction.gaussian_random_sample_)),
    valid_likelihood_(correction.valid_likelihood_),
    likelihood_(std::move(correction.likelihood_))
{ }


GPFCorrection& GPFCorrection::operator=(GPFCorrection&& correction) noexcept
{
    PFCorrection::operator=(std::move(correction));

    gaussian_correction_ = std::move(correction.gaussian_correction_);

    state_model_ = std::move(correction.state_model_);

    generator_ = std::move(correction.generator_);

    distribution_ = std::move(correction.distribution_);

    gaussian_random_sample_ = std::move(correction.gaussian_random_sample_);

    valid_likelihood_ = correction.valid_likelihood_;

    likelihood_ = std::move(correction.likelihood_);

    return *this;
}


MeasurementModel& GPFCorrection::getMeasurementModel() noexcept
{
    return gaussian_correction_->getMeasurementModel();
}


LikelihoodModel& GPFCorrection::getLikelihoodModel() noexcept
{
    return *likelihood_model_;
}


std::pair<bool, Eigen::VectorXd> GPFCorrection::getLikelihood()
{
    return std::make_pair(valid_likelihood_, likelihood_);
}


void GPFCorrection::correctStep(const bfl::ParticleSet& pred_particles, bfl::ParticleSet& corr_particles)
{
    /* Propagate Gaussian belief associated to each particle. */
    gaussian_correction_->correct(pred_particles, corr_particles);

    /* Sample from the proposal distribution. */
    for (std::size_t i = 0; i < pred_particles.components; i++)
    {
        corr_particles.state(i) = sampleFromProposal(corr_particles.mean(i), corr_particles.covariance(i));
    }

    /* Evaluate the likelihood. */
    std::tie(valid_likelihood_, likelihood_) = getLikelihoodModel().likelihood(getMeasurementModel(), corr_particles.state());

    if (!valid_likelihood_)
    {
        corr_particles = pred_particles;

        return;
    }

    /* Evaluate the transition probability. */
    VectorXd transition_probability = state_model_->getTransitionProbability(pred_particles.state(), corr_particles.state());

    /* Update weights in the log space.
     * w_{k} = w_{k-1} + log(likelihood) + log(transition_probability) - log(proposal_distribution)
     */
    double eps = std::numeric_limits<double>::min();
    for (std::size_t i = 0; i < pred_particles.components; i++)
    {
        corr_particles.weight(i) = pred_particles.weight(i) + std::log(likelihood_(i) + eps) + std::log(transition_probability(i) + eps) - std::log(evaluateProposal(corr_particles.state(i), corr_particles.mean(i), corr_particles.covariance(i)) + eps);
    }
}


Eigen::VectorXd GPFCorrection::sampleFromProposal(const Eigen::VectorXd& mean, const Eigen::MatrixXd& covariance)
{
    /* Evaluate the square root of the state covariance matrix using the LDL' decomposition
       (it can be used even if the covariance matrix is positive semidefinite). */
    LDLT<MatrixXd> chol_ldlt(covariance);
    MatrixXd sqrt_P = (chol_ldlt.transpositionsP() * MatrixXd::Identity(mean.size(), mean.size())).transpose() *
                       chol_ldlt.matrixL() *
                       chol_ldlt.vectorD().real().cwiseSqrt().asDiagonal();

    /* Sample i.i.d standard normal univariates. */
    VectorXd rand_vectors(mean.size());
    for (int i = 0; i < rand_vectors.size(); i++)
        rand_vectors(i) = gaussian_random_sample_();

    /* Return a sample from a normal multivariate having mean `mean` and covariance 'covariance'. */
    return mean + sqrt_P * rand_vectors;
}


double GPFCorrection::evaluateProposal
(
    const Eigen::VectorXd& state,
    const Eigen::VectorXd& mean,
    const Eigen::MatrixXd& covariance
)
{
    /* Evaluate the proposal distribution, a Gaussian centered in 'mean' and having
       covariance 'covariance', in the state 'state'. */
    return utils::multivariate_gaussian_density(state, mean, covariance).coeff(0);
}
