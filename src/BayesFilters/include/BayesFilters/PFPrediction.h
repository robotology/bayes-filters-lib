#ifndef PFPREDICTION_H
#define PFPREDICTION_H

#include "StateModel.h"

#include <Eigen/Dense>
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


    bool skip(const bool status);


    virtual StateModel& getStateModel() = 0;

    virtual void setStateModel(std::unique_ptr<StateModel> state_model) = 0;

protected:
    PFPrediction() noexcept;

    PFPrediction(PFPrediction&& pf_prediction) noexcept;


    virtual void predictStep(const Eigen::Ref<const Eigen::MatrixXf>& prev_states, const Eigen::Ref<const Eigen::VectorXf>& prev_weights,
                             Eigen::Ref<Eigen::MatrixXf> pred_states, Eigen::Ref<Eigen::VectorXf> pred_weights) = 0;

private:
    bool skip_ = false;

    friend class PFPredictionDecorator;
};

#endif /* PFPREDICTION_H */
