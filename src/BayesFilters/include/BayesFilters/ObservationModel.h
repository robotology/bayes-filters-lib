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

    virtual void observe(const Eigen::Ref<const Eigen::MatrixXf>& cur_states, Eigen::Ref<Eigen::MatrixXf> observations) = 0;

    virtual void measure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states, Eigen::Ref<Eigen::MatrixXf> measurements) = 0;

    virtual Eigen::MatrixXf getNoiseSample(const int num) = 0;

    virtual Eigen::MatrixXf getNoiseCovariance() = 0;

    virtual bool setProperty(const std::string property) = 0;
};

#endif /* OBSERVATIONMODEL_H */
