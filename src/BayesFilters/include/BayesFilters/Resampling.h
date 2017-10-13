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
    Resampling(unsigned int seed) noexcept;

    Resampling() noexcept;

    Resampling(const Resampling& resampling);

    Resampling(Resampling&& resampling) noexcept;

    ~Resampling() noexcept;

    Resampling& operator=(const Resampling& resampling);

    Resampling& operator=(Resampling&& resampling) noexcept;

    Resampling& operator=(const Resampling&& resampling) noexcept;

    virtual void resample(const Eigen::Ref<const Eigen::MatrixXf>& cor_particles, const Eigen::Ref<const Eigen::VectorXf>& cor_weights,
                          Eigen::Ref<Eigen::MatrixXf> res_particles, Eigen::Ref<Eigen::VectorXf> res_weights, Eigen::Ref<Eigen::VectorXf> res_parents);

    virtual float neff(const Eigen::Ref<const Eigen::VectorXf>& cor_weights);

private:
    std::mt19937_64 generator_;
};

#endif /* RESAMPLING_H */
