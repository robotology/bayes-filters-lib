#ifndef GAUSSIANMIXTURE_H
#define GAUSSIANMIXTURE_H

#include <BayesFilters/GaussianRef.h>

#include <vector>

namespace bfl {
    class GaussianMixture;
}


class bfl::GaussianMixture
{
public:
    GaussianMixture(const std::size_t components, const std::size_t dim);

    GaussianMixture(const std::size_t components, const std::size_t dim_linear, const std::size_t dim_circular);

    virtual ~GaussianMixture() noexcept;

    GaussianRef operator[](const std::size_t i);

    const GaussianConstRef operator[](const std::size_t i) const;

    std::size_t components;

    std::size_t dim;

    std::size_t dim_linear;

    std::size_t dim_circular;

    Eigen::MatrixXd mean;

    Eigen::MatrixXd covariance;

    Eigen::VectorXd weight;
};

#endif /* GAUSSIANMIXTURE_H */
