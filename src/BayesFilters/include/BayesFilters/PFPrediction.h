#ifndef PFPREDICTION_H
#define PFPREDICTION_H

#include "ExogenousModel.h"
#include "StateModel.h"

#include <Eigen/Dense>
#include <memory>
#include <string>

namespace bfl {
    class PFPrediction;
}


class bfl::PFPrediction
{
public:
    virtual ~PFPrediction() noexcept { };

    void predict(const Eigen::Ref<const Eigen::MatrixXf>& prev_states, const Eigen::Ref<const Eigen::VectorXf>& prev_weights,
                 Eigen::Ref<Eigen::MatrixXf> pred_states, Eigen::Ref<Eigen::VectorXf> pred_weights);

    bool skip(const std::string& what_step, const bool status);

    bool getSkipState();

    bool getSkipExogenous();

    virtual void setStateModel(std::unique_ptr<StateModel> state_model) = 0;

    virtual void setExogenousModel(std::unique_ptr<ExogenousModel> exogenous_model);

    /* FIXME
     * This function calls may be deleted in future releases. */
    virtual StateModel& getStateModel() = 0;

    /* FIXME
     * This function calls may be deleted in future releases. */
    virtual ExogenousModel& getExogenousModel();

protected:
    PFPrediction() noexcept;

    PFPrediction(PFPrediction&& pf_prediction) noexcept;

    virtual void predictStep(const Eigen::Ref<const Eigen::MatrixXf>& prev_states, const Eigen::Ref<const Eigen::VectorXf>& prev_weights,
                             Eigen::Ref<Eigen::MatrixXf> pred_states, Eigen::Ref<Eigen::VectorXf> pred_weights) = 0;

private:
    bool skip_prediction_ = false;

    bool skip_state_      = false;

    bool skip_exogenous_  = false;

    friend class PFPredictionDecorator;
};

#endif /* PFPREDICTION_H */
