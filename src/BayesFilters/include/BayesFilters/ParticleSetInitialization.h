#ifndef PARTICLESETINITIALIZATION_H
#define PARTICLESETINITIALIZATION_H

#include <Eigen/Dense>

namespace bfl {
    class ParticleSetInitialization;
}


class bfl::ParticleSetInitialization
{
public:
    virtual ~ParticleSetInitialization() noexcept { };

    virtual bool initialize(Eigen::Ref<Eigen::MatrixXf> states, Eigen::Ref<Eigen::VectorXf> weights) = 0;
};

#endif /* PARTICLESETINITIALIZATION_H */
