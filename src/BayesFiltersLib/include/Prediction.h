#ifndef PREDICTION_H
#define PREDICTION_H

#include <Eigen/Dense>


class Prediction
{
public:
    virtual ~Prediction() noexcept { };

    virtual void predict(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> pred_state) = 0;
};

#endif /* PREDICTION_H */
