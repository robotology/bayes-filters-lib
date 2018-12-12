#include <BayesFilters/KalmanFilter.h>

using namespace bfl;


KalmanFilter::KalmanFilter
(
    Gaussian& initial_state,
    std::unique_ptr<GaussianPrediction> prediction,
    std::unique_ptr<GaussianCorrection> correction
) noexcept :
    prediction_(std::move(prediction)),
    correction_(std::move(correction)),
    predicted_state_(initial_state.dim_linear, initial_state.dim_circular),
    corrected_state_(initial_state)
{ }


KalmanFilter::KalmanFilter(KalmanFilter&& kf) noexcept :
    prediction_(std::move(kf.prediction_)),
    correction_(std::move(kf.correction_)),
    predicted_state_(std::move(kf.predicted_state_)),
    corrected_state_(std::move(kf.corrected_state_))
{ }


KalmanFilter::~KalmanFilter() noexcept
{ }


bool KalmanFilter::initialization()
{
    return true;
}


void KalmanFilter::filteringStep()
{
    prediction_->predict(corrected_state_, predicted_state_);
    correction_->correct(predicted_state_, corrected_state_);
}


bool KalmanFilter::runCondition()
{
    return true;
}


bool KalmanFilter::skip(const std::string& what_step, const bool status)
{
    if (what_step == "prediction" ||
        what_step == "state"      ||
        what_step == "exogenous")
        return prediction_->skip(what_step, status);

    if (what_step == "correction")
        return correction_->skip(status);

    if (what_step == "all")
    {
        bool return_status = true;

        return_status &= prediction_->skip("prediction", status);
        return_status &= prediction_->skip("state", status);
        return_status &= prediction_->skip("exogenous", status);

        return_status &= correction_->skip(status);

        return return_status;
    }

    return false;
}
