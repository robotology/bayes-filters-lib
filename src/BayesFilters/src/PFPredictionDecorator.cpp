#include <BayesFilters/PFPredictionDecorator.h>

#include <utility>

using namespace bfl;
using namespace Eigen;


PFPredictionDecorator::PFPredictionDecorator(std::unique_ptr<PFPrediction> prediction) noexcept :
    prediction_(std::move(prediction)) { }


PFPredictionDecorator::PFPredictionDecorator(PFPredictionDecorator&& prediction) noexcept :
    prediction_(std::move(prediction.prediction_)) { }


PFPredictionDecorator::~PFPredictionDecorator() noexcept { }


PFPredictionDecorator& PFPredictionDecorator::operator=(PFPredictionDecorator&& prediction) noexcept
{
    prediction_ = std::move(prediction.prediction_);

    return *this;
}


void PFPredictionDecorator::predictStep(const Ref<const MatrixXf>& prev_states, const Ref<const VectorXf>& prev_weights,
                                        Ref<MatrixXf> pred_states, Ref<VectorXf> pred_weights)
{
    prediction_->predictStep(prev_states, prev_weights,
                             pred_states, pred_weights);
}


StateModel& PFPredictionDecorator::getStateModel()
{
    return prediction_->getStateModel();
}


void PFPredictionDecorator::setStateModel(std::unique_ptr<StateModel> state_model)
{
    prediction_->setStateModel(std::move(state_model));
}


ExogenousModel& PFPredictionDecorator::getExogenousModel()
{
    return prediction_->getExogenousModel();
}


void PFPredictionDecorator::setExogenousModel(std::unique_ptr<ExogenousModel> exogenous_model)
{
    prediction_->setExogenousModel(std::move(exogenous_model));
}
