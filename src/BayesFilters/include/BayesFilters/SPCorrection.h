#ifndef SPCORRECTION_H
#define SPCORRECTION_H

#include "ObservationModel.h"

#include <memory>
#include <vector>

#include <Eigen/Dense>

namespace bfl {
    class SPCorrection;
}


class bfl::SPCorrection
{
public:
    virtual ~SPCorrection() noexcept { };


    void correct(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const std::vector<Eigen::MatrixXf>& pred_covariances, const Eigen::Ref<const Eigen::MatrixXf>& measurements,
                 Eigen::Ref<Eigen::MatrixXf> cor_states, std::vector<Eigen::MatrixXf>& cor_covariances);


    bool skip(const bool status);


    virtual ObservationModel& getObservationModel() = 0;

    virtual void setObservationModel(std::unique_ptr<ObservationModel> observation_model) = 0;

protected:
    SPCorrection() noexcept;

    SPCorrection(SPCorrection&& pf_correction) noexcept;


    virtual void correctStep(const Eigen::Ref<const Eigen::MatrixXf>& prev_states, const std::vector<Eigen::MatrixXf>& prev_covariances, const Eigen::Ref<const Eigen::MatrixXf>& measurements,
                             Eigen::Ref<Eigen::MatrixXf> cor_states, std::vector<Eigen::MatrixXf>& cor_covariances) = 0;

    virtual void innovation(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::MatrixXf> innovations) = 0;

    virtual double likelihood(const Eigen::Ref<const Eigen::VectorXf>& innovation, const Eigen::Ref<const Eigen::MatrixXf>& innovation_covariance) = 0;

private:
    bool skip_ = false;

    friend class SPCorrectionDecorator;
};

#endif /* SPCORRECTION_H */
