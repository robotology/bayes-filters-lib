#ifndef RESAMPLING_H
#define RESAMPLING_H

#include <random>

#include <BayesFilters/ParticleSet.h>

#include <Eigen/Dense>

namespace bfl {
    class Resampling;
}


class bfl::Resampling
{
public:
    Resampling(unsigned int seed) noexcept;

    Resampling() noexcept;

    Resampling(const Resampling& resampling) noexcept;

    Resampling(Resampling&& resampling) noexcept;

    virtual ~Resampling() noexcept;

    Resampling& operator=(const Resampling& resampling);

    Resampling& operator=(Resampling&& resampling) noexcept;

    Resampling& operator=(const Resampling&& resampling) noexcept;

    virtual void resample(const bfl::ParticleSet& cor_particles, bfl::ParticleSet& res_particles,
                          Eigen::Ref<Eigen::VectorXi> res_parents);

    virtual double neff(const Eigen::Ref<const Eigen::VectorXd>& cor_weights);

private:
    std::mt19937_64 generator_;
};

#endif /* RESAMPLING_H */
