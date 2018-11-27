#ifndef PARTICLESET_H
#define PARTICLESET_H

#include <BayesFilters/GaussianMixture.h>

#include <vector>

namespace bfl {
    class ParticleSet;
}


class bfl::ParticleSet : public bfl::GaussianMixture
{
public:
    ParticleSet(const std::size_t components, const std::size_t dim) noexcept;

    ParticleSet(const std::size_t components, const std::size_t dim_linear, const std::size_t dim_circular) noexcept;

    virtual ~ParticleSet() noexcept;

    ParticleSet& operator+=(const ParticleSet& rhs);

    Eigen::Ref<Eigen::MatrixXd> state();

    Eigen::Ref<Eigen::MatrixXd> state(const std::size_t i);

    double& state(const std::size_t i, const std::size_t j);

    const Eigen::Ref<const Eigen::MatrixXd> state() const;

    const Eigen::Ref<const Eigen::MatrixXd> state(const std::size_t i) const;

    const double& state(const std::size_t i, const std::size_t j) const;

protected:
    Eigen::MatrixXd state_;
};

bfl::ParticleSet operator+(bfl::ParticleSet lhs, const bfl::ParticleSet& rhs);

#endif /* PARTICLESET_H */
