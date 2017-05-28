#ifndef RESAMPLING_H
#define RESAMPLING_H

#include <random>

#include <Eigen/Dense>

namespace bfl {
    class Resampling;
}


class bfl::Resampling
{
public:
    /* Resampling complete constructor */
    Resampling(unsigned int seed) noexcept;

    /* Default constructor */
    Resampling() noexcept;

    /* Destructor */
    ~Resampling() noexcept;

    /* Copy constructor */
    Resampling(const Resampling& resampling);

    /* Move constructor */
    Resampling(Resampling&& resampling) noexcept;

    /* Copy assignment operator */
    Resampling& operator=(const Resampling& resampling);

    /* Move assignment operator */
    Resampling& operator=(Resampling&& resampling) noexcept;

    /* Move assignment operator */
    Resampling& operator=(const Resampling&& resampling) noexcept;

    virtual void resample(const Eigen::Ref<const Eigen::MatrixXf>& pred_particles, const Eigen::Ref<const Eigen::VectorXf>& cor_weights,
                          Eigen::Ref<Eigen::MatrixXf> res_particles, Eigen::Ref<Eigen::VectorXf> res_weights, Eigen::Ref<Eigen::VectorXf> res_parents);

    virtual float neff(const Eigen::Ref<const Eigen::VectorXf>& cor_weights);

private:
    std::mt19937_64 generator_;
};

#endif /* RESAMPLING_H */
