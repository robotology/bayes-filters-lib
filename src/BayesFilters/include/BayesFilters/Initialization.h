#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include <Eigen/Dense>

namespace bfl {
    class Initialization;
}


class bfl::Initialization
{
public:
    virtual ~Initialization() noexcept { };

    virtual void initialize(Eigen::Ref<Eigen::MatrixXf> states, Eigen::Ref<Eigen::VectorXf> weights) = 0;
};

#endif /* INITIALIZATION_H */
