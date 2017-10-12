#ifndef PFPREDICTIONDECORATOR_H
#define PFPREDICTIONDECORATOR_H

#include "PFPrediction.h"

#include <memory>

namespace bfl {
    class PFPredictionDecorator;
}


class bfl::PFPredictionDecorator : public PFPrediction
{
public:
    void predict(const Eigen::Ref<const Eigen::MatrixXf>& prev_state, Eigen::Ref<Eigen::MatrixXf> pred_state) override;

protected:
    PFPredictionDecorator(std::unique_ptr<PFPrediction> prediction) noexcept;

    PFPredictionDecorator(PFPredictionDecorator&& prediction) noexcept;

    ~PFPredictionDecorator() noexcept;

    PFPredictionDecorator& operator=(PFPredictionDecorator&& prediction) noexcept;

private:
    std::unique_ptr<PFPrediction> prediction_;
};

#endif /* PFPREDICTIONDECORATOR_H */
