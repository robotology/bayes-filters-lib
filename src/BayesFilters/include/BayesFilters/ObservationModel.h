#ifndef OBSERVATIONMODEL_H
#define OBSERVATIONMODEL_H

#include <Eigen/Dense>
#include <string>

namespace bfl {
    class ObservationModel;
}


class bfl::ObservationModel
{
public:
    virtual ~ObservationModel() noexcept { };

    virtual void observe(const Eigen::Ref<const Eigen::MatrixXf>& cur_state, Eigen::Ref<Eigen::MatrixXf> observation) = 0;

    virtual void measure(const Eigen::Ref<const Eigen::MatrixXf>& cur_state, Eigen::Ref<Eigen::MatrixXf> measurement) = 0;

    virtual Eigen::MatrixXf getNoiseSample(const int num) = 0;

    virtual Eigen::MatrixXf getNoiseCovariance() = 0;

    virtual bool setProperty(const std::string property) = 0;
};

#endif /* OBSERVATIONMODEL_H */
