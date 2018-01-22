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
    GaussianMixture(unsigned int num_component, unsigned int dim);

    virtual ~GaussianMixture();

    Gaussian&       operator[](unsigned int i);
    const Gaussian& operator[](unsigned int i) const;

private:
    std::vector<Gaussian> gaussian_;
};

#endif /* GAUSSIANMIXTURE_H */
