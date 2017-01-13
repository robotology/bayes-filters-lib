#ifndef OBSERVATIONMODEL_H
#define OBSERVATIONMODEL_H

#include <Eigen/Dense>


class ObservationModel
{
public:

    ObservationModel() = default;

    virtual ~ObservationModel() noexcept { };

    virtual Eigen::MatrixXf noiseCovariance() = 0;

    virtual void observe(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> observation) = 0;

    virtual void noiseSample(Eigen::Ref<Eigen::VectorXf> sample) = 0;

    virtual void measure(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> measurement) = 0;
};

#endif /* OBSERVATIONMODEL_H */
