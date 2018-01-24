#include "BayesFilters/ResamplingWithPrior.h"

#include <algorithm>
#include <numeric>
#include <vector>

using namespace bfl;
using namespace Eigen;


ResamplingWithPrior::ResamplingWithPrior(std::unique_ptr<bfl::Initialization> init_model, const double prior_ratio, const unsigned int seed) noexcept :
    Resampling(seed),
    init_model_(std::move(init_model)),
    prior_ratio_(prior_ratio) { }


ResamplingWithPrior::ResamplingWithPrior(std::unique_ptr<Initialization> init_model, const double prior_ratio) noexcept :
    Resampling(1),
    init_model_(std::move(init_model)),
    prior_ratio_(prior_ratio)  { }


ResamplingWithPrior::ResamplingWithPrior(std::unique_ptr<Initialization> init_model) noexcept :
    Resampling(1),
    init_model_(std::move(init_model)) { }


ResamplingWithPrior::ResamplingWithPrior(ResamplingWithPrior&& resampling) noexcept :
    Resampling(std::move(resampling)),
    init_model_(std::move(resampling.init_model_))
{
    prior_ratio_ = resampling.prior_ratio_;
    resampling.prior_ratio_ = 0.5;
}


ResamplingWithPrior::~ResamplingWithPrior() noexcept { }


ResamplingWithPrior& ResamplingWithPrior::operator=(ResamplingWithPrior&& resampling) noexcept
{
    if (this != &resampling)
    {
        Resampling::operator=(std::move(resampling));

        init_model_ = std::move(resampling.init_model_);
    }

    return *this;
}


void ResamplingWithPrior::resample(const Ref<const MatrixXf>& pred_particles, const Ref<const VectorXf>& cor_weights,
                                   Ref<MatrixXf> res_particles, Ref<VectorXf> res_weights, Ref<VectorXf> res_parents)
{
    int num_prior_particles    = static_cast<int>(std::floor(pred_particles.cols() * prior_ratio_));
    int num_resample_particles = pred_particles.cols() - num_prior_particles;

    MatrixXf tmp_particles(pred_particles.rows(), num_resample_particles);
    VectorXf tmp_weights(num_resample_particles);

    int j = 0;
    for (int i: sort_indices(cor_weights))
    {
        if (j < num_prior_particles)
        {
            res_particles.col(j) = pred_particles.col(i);
            res_weights(j)       = cor_weights(i);
        }
        else
        {
            tmp_particles.col(j - num_prior_particles) = pred_particles.col(i);
            tmp_weights(j - num_prior_particles)       = cor_weights(i);
        }
        j++;
    }

    init_model_->initialize(res_particles.leftCols(num_prior_particles), res_weights.head(num_prior_particles));

    Resampling::resample(tmp_particles, tmp_weights / tmp_weights.sum(),
                         res_particles.rightCols(num_resample_particles), res_weights.tail(num_resample_particles), res_parents);

    res_weights.setConstant(1.0 / pred_particles.cols());
}


std::vector<unsigned int> ResamplingWithPrior::sort_indices(const Ref<const VectorXf>& vector)
{
    std::vector<unsigned int> idx(vector.size());
    std::iota(idx.begin(), idx.end(), 0);

    std::sort(idx.begin(), idx.end(),
              [&vector](size_t idx1, size_t idx2) { return vector[idx1] < vector[idx2]; });

    return idx;
}
