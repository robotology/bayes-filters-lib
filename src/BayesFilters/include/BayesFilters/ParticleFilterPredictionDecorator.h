#ifndef PFPREDICTIONDECORATOR_H
#define PFPREDICTIONDECORATOR_H

#include <memory>

#include "ParticleFilterPrediction.h"

namespace bfl {
    class ParticleFilterPredictionDecorator;
}


class bfl::ParticleFilterPredictionDecorator : public ParticleFilterPrediction
{
public:
    void predict(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> pred_state) override;

protected:
    ParticleFilterPredictionDecorator(std::unique_ptr<ParticleFilterPrediction> prediction) noexcept;

    ParticleFilterPredictionDecorator(ParticleFilterPredictionDecorator&& prediction) noexcept;

    ~ParticleFilterPredictionDecorator() noexcept override;

    ParticleFilterPredictionDecorator& operator=(ParticleFilterPredictionDecorator&& prediction) noexcept;

private:
    std::unique_ptr<ParticleFilterPrediction> prediction_;
};

#endif /* PFPREDICTIONDECORATOR_H */
