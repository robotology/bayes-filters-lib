#ifndef PARTICLE_H
#define PARTICLE_H

#include <Eigen/Core>

namespace bfl {
    class Particle;
}


class bfl::Particle
{
public:
    Particle(unsigned int dim);

    virtual ~Particle();

    double weight;

    Eigen::VectorXd state;
};

#endif /* PARTICLE_H */
