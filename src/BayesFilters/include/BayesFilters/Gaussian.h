#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <Eigen/Core>

namespace bfl {
    class Gaussian;
}


class bfl::Gaussian
{
public:
    Gaussian(unsigned int dim);

    virtual ~Gaussian();

    Eigen::VectorXd mean;

    Eigen::MatrixXd covariance;
};

#endif /* GAUSSIAN_H */
