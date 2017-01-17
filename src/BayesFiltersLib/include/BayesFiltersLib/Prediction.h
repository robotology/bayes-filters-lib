#ifndef PREDICTION_H
#define PREDICTION_H

#include <Eigen/Dense>


namespace bfl
{

class Prediction
{
public:
    virtual ~Prediction() noexcept { };

    virtual void predict(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> pred_state) = 0;
};

} // namespace bfl

#endif /* PREDICTION_H */
