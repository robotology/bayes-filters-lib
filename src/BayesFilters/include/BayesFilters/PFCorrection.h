#ifndef PFCORRECTION_H
#define PFCORRECTION_H

#include "ObservationModel.h"

#include <memory>

#include <Eigen/Dense>

namespace bfl {
    class PFCorrection;
}


class bfl::PFCorrection
{
public:
    virtual ~PFCorrection() noexcept { };


    void correct(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights, const Eigen::Ref<const Eigen::MatrixXf>& measurements,
                 Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights);

    virtual void innovation(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::MatrixXf> innovations) = 0;

    virtual double likelihood(const Eigen::Ref<const Eigen::VectorXf>& innovation) = 0;


    bool skip(const bool status);


    virtual ObservationModel& getObservationModel() = 0;

    virtual void setObservationModel(std::unique_ptr<ObservationModel> observation_model) = 0;

protected:
    PFCorrection() noexcept;

    PFCorrection(PFCorrection&& pf_correction) noexcept;


    virtual void correctStep(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights, const Eigen::Ref<const Eigen::MatrixXf>& measurements,
                             Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) = 0;

private:
    bool skip_ = false;

    friend class PFCorrectionDecorator;
};

#endif /* PFCORRECTION_H */
