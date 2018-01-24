#ifndef RESAMPLINGWITHPRIOR_H
#define RESAMPLINGWITHPRIOR_H

#include <memory>
#include <vector>

#include <BayesFilters/Initialization.h>
#include <BayesFilters/Resampling.h>
#include <Eigen/Dense>

namespace bfl {
    class ResamplingWithPrior;
}


class bfl::ResamplingWithPrior : public Resampling
{
public:
    ResamplingWithPrior(std::unique_ptr<bfl::Initialization> init_model, const double prior_ratio, const unsigned int seed) noexcept;

    ResamplingWithPrior(std::unique_ptr<bfl::Initialization> init_model, const double prior_ratio) noexcept;

    ResamplingWithPrior(std::unique_ptr<bfl::Initialization> init_model) noexcept;

    ResamplingWithPrior(ResamplingWithPrior&& resampling) noexcept;

    virtual ~ResamplingWithPrior() noexcept;

    ResamplingWithPrior& operator=(ResamplingWithPrior&& resampling) noexcept;


    void resample(const Eigen::Ref<const Eigen::MatrixXf>& pred_particles, const Eigen::Ref<const Eigen::VectorXf>& cor_weights,
                  Eigen::Ref<Eigen::MatrixXf> res_particles, Eigen::Ref<Eigen::VectorXf> res_weights, Eigen::Ref<Eigen::VectorXf> res_parents) override;

protected:
    std::unique_ptr<bfl::Initialization> init_model_;

    double prior_ratio_ = 0.5;

private:
    std::vector<unsigned int> sort_indices(const Eigen::Ref<const Eigen::VectorXf>& vector);
};

#endif /* RESAMPLINGWITHPRIOR_H */
