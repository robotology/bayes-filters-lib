#ifndef PFCORRECTION_H
#define PFCORRECTION_H

#include <BayesFilters/MeasurementModel.h>
#include <BayesFilters/LikelihoodModel.h>

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

    /* This is needed to be able to use decorator on correction classes. */
    virtual void setMeasurementModel(std::unique_ptr<MeasurementModel> observation_model) = 0;

    /* This is needed to be able to use decorator on correction classes. */
    virtual void setLikelihoodModel(std::unique_ptr<LikelihoodModel> observation_model) = 0;

    virtual std::pair<bool, Eigen::VectorXf> getLikelihood();

protected:
    /* This is needed to be able to use decorator on correction classes. */
    virtual MeasurementModel& getMeasurementModel() = 0;

    /* This is needed to be able to use decorator on correction classes. */
    virtual LikelihoodModel& getLikelihoodModel() = 0;

    virtual void correctStep(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights,
                             Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) = 0;

    virtual std::pair<bool, Eigen::VectorXf> likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations) = 0;

    PFCorrection() noexcept;

private:
    bool skip_ = false;

    friend class PFCorrectionDecorator;
};

#endif /* PFCORRECTION_H */
