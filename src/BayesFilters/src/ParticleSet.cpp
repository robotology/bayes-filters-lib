#include <BayesFilters/ParticleSet.h>

using namespace bfl;
using namespace Eigen;


ParticleSet::ParticleSet(const std::size_t components, const std::size_t dim) noexcept:
    ParticleSet(components, dim, 0) { }


ParticleSet::ParticleSet
(
    const std::size_t components,
    const std::size_t dim_linear,
    const std::size_t dim_circular
) noexcept :
    GaussianMixture(components, dim_linear, dim_circular),
    state_(dim, components) { }


ParticleSet::~ParticleSet() noexcept { }


ParticleSet ParticleSet::operator+(const ParticleSet& rhs)
{
    /* Should check whether (this->dim_linear == rhs.dim_linear) &&
       (this->dim_circular == rhs.dim_circular). */
    ParticleSet sum(components + rhs.components, dim_linear, dim_circular);

    sum.mean_ << mean_, rhs.mean_;
    sum.covariance_ << covariance_, rhs.covariance_;
    sum.weight_ << weight_, rhs.weight_;
    sum.state_ << state_, rhs.state_;

    return sum;
}


Ref<MatrixXd> ParticleSet::state()
{
    return state_;
}


Ref<MatrixXd> ParticleSet::state(const std::size_t i)
{
    return state_.col(i);
}


const Ref<const MatrixXd> ParticleSet::state() const
{
    return state_;
}


const Ref<const MatrixXd> ParticleSet::state(const std::size_t i) const
{
    return state_.col(i);
}
