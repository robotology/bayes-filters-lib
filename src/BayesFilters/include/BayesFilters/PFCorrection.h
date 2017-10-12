#ifndef PFCORRECTION_H
#define PFCORRECTION_H

#include "ObservationModel.h"

#include <Eigen/Dense>

namespace bfl {
    class PFCorrection;
}


class bfl::PFCorrection
{
public:
    virtual ~PFCorrection() noexcept { };

    virtual void correct(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights, const Eigen::Ref<const Eigen::MatrixXf>& measurements,
                         Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) = 0;

    virtual void innovation(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::MatrixXf> innovations) = 0;

    virtual double likelihood(const Eigen::Ref<const Eigen::VectorXf>& innovation) = 0;

    virtual ObservationModel& getObservationModel() = 0;
};

#endif /* PFCORRECTION_H */
