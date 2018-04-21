#ifndef PFCORRECTION_H
#define PFCORRECTION_H

#include <BayesFilters/MeasurementModel.h>

#include <memory>
#include <utility>

#include <Eigen/Dense>

namespace bfl {
    class PFCorrection;
}


class bfl::PFCorrection
{
public:
    virtual ~PFCorrection() noexcept { };

    void correct(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights,
                 Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights);

    bool skip(const bool status);

    /* FIXME
       While setObservationModel() will be kept in future implementation, measurement_model_ member is
       currently set with public visibility for backward compatibility. It will be moved to private in future releases. */
    void setMeasurementModel(std::unique_ptr<MeasurementModel> observation_model);

    std::unique_ptr<MeasurementModel> measurement_model_;

    virtual std::pair<bool, Eigen::VectorXf> getLikelihood();

protected:
    virtual void correctStep(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights,
                             Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) = 0;

    virtual std::pair<bool, Eigen::VectorXf> likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations) = 0;

    PFCorrection() noexcept;

    PFCorrection(PFCorrection&& pf_correction) noexcept;

private:
    bool skip_ = false;

    friend class PFCorrectionDecorator;
};

#endif /* PFCORRECTION_H */
