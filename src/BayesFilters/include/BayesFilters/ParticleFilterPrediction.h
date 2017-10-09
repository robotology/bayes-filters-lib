#ifndef PFPREDICTION_H
#define PFPREDICTION_H

#include "StateModel.h"

#include <Eigen/Dense>
#include <string>

namespace bfl {
    class ParticleFilterPrediction;
}


class bfl::ParticleFilterPrediction
{
public:
    virtual ~ParticleFilterPrediction() noexcept { };

    virtual void predict(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> pred_state) = 0;

    virtual StateModel getStateModel() = 0;
};

#endif /* PFPREDICTION_H */
