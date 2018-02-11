#ifndef GAUSSIANMIXTURE_H
#define GAUSSIANMIXTURE_H

#include "BayesFilters/Gaussian.h"

#include <vector>

namespace bfl {
    class GaussianMixture;
}


class bfl::GaussianMixture
{
public:
    GaussianMixture(const unsigned int components, const unsigned int dim);

    GaussianMixture(const unsigned int components, const unsigned int dim_linear, const unsigned int dim_circular);

    virtual ~GaussianMixture() noexcept;

    Gaussian&       operator[](unsigned int i);
    const Gaussian& operator[](unsigned int i) const;

    unsigned int components;

    unsigned int dim;

    unsigned int dim_linear;

    unsigned int dim_circular;

    Eigen::MatrixXd means;

    Eigen::MatrixXd covariances;

    Eigen::VectorXd weights;

private:
    std::vector<Gaussian> gaussian_;
};

#endif /* GAUSSIANMIXTURE_H */
