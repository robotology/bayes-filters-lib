#include <BayesFilters/Resampling.h>

#include <utility>

using namespace bfl;
using namespace Eigen;


Resampling::Resampling(unsigned int seed) noexcept :
    generator_(std::mt19937_64(seed)) { }


Resampling::Resampling() noexcept :
    Resampling(1) { }


Resampling::~Resampling() noexcept { }


Resampling::Resampling(const Resampling& resampling) noexcept :
    generator_(resampling.generator_) { }


Resampling::Resampling(Resampling&& resampling) noexcept :
    generator_(std::move(resampling.generator_)) { }


Resampling& Resampling::operator=(const Resampling& resampling)
{
    Resampling tmp(resampling);
    *this = std::move(tmp);

    return *this;
}


Resampling& Resampling::operator=(Resampling&& resampling) noexcept
{
    generator_ = std::move(resampling.generator_);

    return *this;
}


Resampling& Resampling::operator=(const Resampling&& resampling) noexcept
{
    generator_ = std::move(resampling.generator_);

    return *this;
}


void Resampling::resample(const Ref<const MatrixXf>& cor_particles, const Ref<const VectorXf>& cor_weights,
                          Ref<MatrixXf> res_particles, Ref<VectorXf> res_weights, Ref<VectorXf> res_parents)
{
    int num_particles = static_cast<int>(cor_weights.rows());
    VectorXf csw(num_particles);

    csw(0) = cor_weights(0);
    for (int i = 1; i < num_particles; ++i)
        csw(i) = csw(i-1) + cor_weights(i);

    std::uniform_real_distribution<float> distribution_res(0.0, 1.0/num_particles);
    float u_1 = distribution_res(generator_);

    int idx_csw = 0;
    for (int j = 0; j < num_particles; ++j)
    {
        float u_j = u_1 + static_cast<float>(j)/num_particles;

        while (u_j > csw(idx_csw) && idx_csw < (num_particles - 1))
            idx_csw += 1;

        res_particles.col(j) = cor_particles.col(idx_csw);
        res_weights(j)       = 1.0/num_particles;
        res_parents(j)       = idx_csw;
    }
}


float Resampling::neff(const Ref<const VectorXf>& cor_weights)
{
    return 1.0/cor_weights.array().square().sum();
}
