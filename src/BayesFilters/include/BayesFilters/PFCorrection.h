#ifndef PFCORRECTION_H
#define PFCORRECTION_H

#include <BayesFilters/LikelihoodModel.h>
#include <BayesFilters/MeasurementModel.h>
#include <BayesFilters/Process.h>

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

    virtual void setLikelihoodModel(std::unique_ptr<LikelihoodModel> observation_model) = 0;

    virtual void setMeasurementModel(std::unique_ptr<MeasurementModel> measurement_model) = 0;

    virtual void setProcess(std::unique_ptr<Process> process) = 0;

    virtual std::pair<bool, Eigen::VectorXf> getLikelihood() = 0;

protected:
    /* FIXME
       There may no need for the folloiwng method to exist.
       Possibly removed in future versions. */
    virtual LikelihoodModel& getLikelihoodModel() = 0;

    /* FIXME
     There may no need for the folloiwng method to exist.
     Possibly removed in future versions. */
    virtual MeasurementModel& getMeasurementModel() = 0;

    /* FIXME
     There may no need for the folloiwng method to exist.
     Possibly removed in future versions. */
    virtual Process& getProcess() = 0;

    virtual void correctStep(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights,
                             Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) = 0;

    PFCorrection() noexcept;

private:
    bool skip_ = false;

    friend class PFCorrectionDecorator;
};

#endif /* PFCORRECTION_H */
