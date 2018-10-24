#ifndef PFPREDICTIONDECORATOR_H
#define PFPREDICTIONDECORATOR_H

#include <BayesFilters/PFPrediction.h>

#include <memory>

namespace bfl {
    class PFPredictionDecorator;
}


class bfl::PFPredictionDecorator : public PFPrediction
{
protected:
    PFPredictionDecorator(std::unique_ptr<PFPrediction> prediction) noexcept;

    PFPredictionDecorator(PFPredictionDecorator&& prediction) noexcept;

    virtual ~PFPredictionDecorator() noexcept;

    PFPredictionDecorator& operator=(PFPredictionDecorator&& prediction) noexcept;

    StateModel& getStateModel() override;

    void setStateModel(std::unique_ptr<StateModel> state_model) override;

    ExogenousModel& getExogenousModel() override;

    void setExogenousModel(std::unique_ptr<ExogenousModel> exogenous_model) override;

protected:
    void predictStep(const Eigen::Ref<const Eigen::MatrixXf>& prev_states, const Eigen::Ref<const Eigen::VectorXf>& prev_weights,
                     Eigen::Ref<Eigen::MatrixXf> pred_states, Eigen::Ref<Eigen::VectorXf> pred_weights) override;

private:
    std::unique_ptr<PFPrediction> prediction_;
};

#endif /* PFPREDICTIONDECORATOR_H */
