#ifndef OBSERVATIONMODEL_H
#define OBSERVATIONMODEL_H

#include <Eigen/Dense>
#include <string>


namespace bfl
{

class ObservationModel
{
public:
    virtual ~ObservationModel() noexcept { };

    virtual Eigen::MatrixXf noiseCovariance() = 0;

    virtual void observe(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::MatrixXf> observation) = 0;

    virtual void noiseSample(Eigen::Ref<Eigen::VectorXf> sample) = 0;

    virtual bool setProperty(const std::string property) = 0;

    virtual void measure(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::MatrixXf> measurement) = 0;
};

} // namespace bfl

#endif /* OBSERVATIONMODEL_H */
