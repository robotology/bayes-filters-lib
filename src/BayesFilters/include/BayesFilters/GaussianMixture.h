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

    virtual ~GaussianMixture() noexcept;

    Gaussian&       operator[](unsigned int i);
    const Gaussian& operator[](unsigned int i) const;

    Eigen::MatrixXd means;

    Eigen::MatrixXd covariances;

    Eigen::VectorXd weights;

    unsigned int components;

    unsigned int dim;

private:
    std::vector<Gaussian> gaussian_;
};

#endif /* GAUSSIANMIXTURE_H */
