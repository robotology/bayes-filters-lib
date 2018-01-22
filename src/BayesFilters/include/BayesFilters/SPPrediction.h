#ifndef SPPREDICTION_H
#define SPPREDICTION_H

#include "ExogenousModel.h"
#include "StateModel.h"

#include <Eigen/Dense>
#include <memory>
#include <string>
#include <vector>

namespace bfl {
    class SPPrediction;
}


class bfl::SPPrediction
{
public:
    virtual ~SPPrediction() noexcept { };


    void predict(const Eigen::Ref<const Eigen::MatrixXf>& prev_states, const std::vector<Eigen::MatrixXf>& prev_covariances,
                 Eigen::Ref<Eigen::MatrixXf> pred_states, std::vector<Eigen::MatrixXf>& pred_weights);


    bool skip(const std::string& what_step, const bool status);

    bool getSkipState();

    bool getSkipExogenous();


    virtual StateModel& getStateModel() = 0;

    virtual void setStateModel(std::unique_ptr<StateModel> state_model) = 0;

    virtual ExogenousModel& getExogenousModel();

    virtual void setExogenousModel(std::unique_ptr<ExogenousModel> exogenous_model);

protected:
    SPPrediction() noexcept;

    SPPrediction(SPPrediction&& pf_prediction) noexcept;


    virtual void predictStep(const Eigen::Ref<const Eigen::MatrixXf>& prev_states, const std::vector<Eigen::MatrixXf>& prev_covariances,
                             Eigen::Ref<Eigen::MatrixXf> pred_states, std::vector<Eigen::MatrixXf>& pred_weights) = 0;

private:
    bool skip_prediction_ = false;

    bool skip_state_      = false;

    bool skip_exogenous_  = false;

    friend class SPPredictionDecorator;
};

#endif /* SPPREDICTION_H */
