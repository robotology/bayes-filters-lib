#ifndef LIKELIHOODMODEL_H
#define LIKELIHOODMODEL_H

#include <BayesFilters/MeasurementModel.h>

#include <Eigen/Core>

namespace bfl {
    class LikelihoodModel;
}


class bfl::LikelihoodModel
{
public:
    virtual ~LikelihoodModel() noexcept { };

    virtual std::pair<bool, Eigen::VectorXf> likelihood(const MeasurementModel& measurement_model, const Eigen::Ref<const Eigen::MatrixXf>& pred_states) = 0;
};

#endif /* LIKELIHOODMODEL_H */
