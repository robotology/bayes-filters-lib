#ifndef UKFPREDICTION_H
#define UKFPREDICTION_H

#include "SPPrediction.h"

#include <memory>

namespace bfl {
    class UKFPrediction;
}


class bfl::UKFPrediction: public SPPrediction
{
public:
    UKFPrediction() noexcept;

    UKFPrediction(UKFPrediction&& ukf_prediction) noexcept;

    virtual ~UKFPrediction() noexcept;

    virtual StateModel& getStateModel() override;

    virtual void setStateModel(std::unique_ptr<StateModel> state_model) override;

protected:
    Gaussian predictStep(const Gaussian& prev_states) override;

    std::unique_ptr<StateModel> state_model_;
};

#endif /* UKFPREDICTION_H */
