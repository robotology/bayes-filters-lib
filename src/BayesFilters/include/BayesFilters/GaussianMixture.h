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
    GaussianMixture(unsigned int components, unsigned int dim);

    virtual ~GaussianMixture() noexcept;

    Gaussian&       operator[](unsigned int i);
    const Gaussian& operator[](unsigned int i) const;

    unsigned int components;

    unsigned int dim;

private:
    Eigen::MatrixXd mean;

    std::vector<Eigen::MatrixXd> covariance;

    Eigen::VectorXd weight;

    std::vector<Gaussian> gaussian_;
};

#endif /* GAUSSIANMIXTURE_H */
