#ifndef SPCORRECTION_H
#define SPCORRECTION_H

#include "Gaussian.h"
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


    Gaussian correct(const Gaussian& pred_state, const Eigen::Ref<const Eigen::MatrixXf>& measurements);


    bool skip(const bool status);


    virtual ObservationModel& getObservationModel() = 0;

    virtual void setObservationModel(std::unique_ptr<ObservationModel> observation_model) = 0;

protected:
    SPCorrection() noexcept;

    SPCorrection(SPCorrection&& pf_correction) noexcept;


    virtual Gaussian correctStep(const Gaussian& prev_state, const Eigen::Ref<const Eigen::MatrixXf>& measurements) = 0;

    virtual Eigen::MatrixXf innovation(const Gaussian& pred_state, const Eigen::Ref<const Eigen::MatrixXf>& measurements) = 0;

    virtual double likelihood(const Eigen::Ref<const Eigen::VectorXf>& innovation, const Eigen::Ref<const Eigen::MatrixXf>& innovation_covariance) = 0;

private:
    bool skip_ = false;

    friend class SPCorrectionDecorator;
};

#endif /* SPCORRECTION_H */
