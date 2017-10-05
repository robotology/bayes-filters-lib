#ifndef CORRECTION_H
#define CORRECTION_H

#include "AbstractCorrection.h"
#include "ObservationModel.h"

namespace bfl {
    class Correction;
}


class bfl::Correction : public AbstractCorrection
{
public:
    Correction(std::unique_ptr<ObservationModel> obs_model) noexcept :
        obs_model_(std::move(obs_model))
    { }

    virtual ~Correction() noexcept { };

    Correction(Correction&& pf_correction) noexcept :
        obs_model_(std::move(pf_correction.obs_model_))
    { }

    Correction& operator=(Correction&& pf_correction) noexcept
    {
        obs_model_ = std::move(pf_correction.obs_model_);

        return *this;
    }

    virtual void observeState(const Eigen::Ref<const Eigen::MatrixXf>& states, Eigen::Ref<Eigen::MatrixXf> observations) override
    {
        obs_model_->observe(states, observations);
    }

    void measureState(const Eigen::Ref<const Eigen::MatrixXf>& states, Eigen::Ref<Eigen::MatrixXf> measurements) override
    {
        obs_model_->measure(states, measurements);
    }

protected:
    std::unique_ptr<ObservationModel> obs_model_;
};

#endif /* CORRECTION_H */
