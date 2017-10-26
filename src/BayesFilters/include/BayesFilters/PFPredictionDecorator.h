#ifndef PFPREDICTIONDECORATOR_H
#define PFPREDICTIONDECORATOR_H

#include "PFPrediction.h"

#include <memory>

namespace bfl {
    class PFPredictionDecorator;
}


class bfl::PFPredictionDecorator : public PFPrediction
{
protected:
    PFPredictionDecorator(std::unique_ptr<PFPrediction> prediction) noexcept;

    PFPredictionDecorator(PFPredictionDecorator&& prediction) noexcept;

    ~PFPredictionDecorator() noexcept;

    PFPredictionDecorator& operator=(PFPredictionDecorator&& prediction) noexcept;


    void predictStep(const Eigen::Ref<const Eigen::MatrixXf>& prev_states, const Eigen::Ref<const Eigen::VectorXf>& prev_weights,
                     Eigen::Ref<Eigen::MatrixXf> pred_states, Eigen::Ref<Eigen::VectorXf> pred_weights) override;

private:
    std::unique_ptr<PFPrediction> prediction_;
};

#endif /* PFPREDICTIONDECORATOR_H */
