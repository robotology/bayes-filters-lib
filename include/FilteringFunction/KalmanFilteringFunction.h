#ifndef KALMANFILTERINGFUNCTION_H
#define KALMANFILTERINGFUNCTION_H

#include "BayesianFilteringFunction.h"


class KalmanFilteringFunction: public BayesianFilteringFunction {

    virtual void StateModel(const Eigen::Ref<const Eigen::VectorXf> & prev_state, Eigen::Ref<Eigen::VectorXf> prop_state);

    virtual Eigen::Ref<Eigen::MatrixXf> ObservationModel(const Eigen::Ref<const Eigen::VectorXf> & pred_state);

    virtual void Prediction(const Eigen::Ref<const Eigen::VectorXf> & prev_state, Eigen::Ref<Eigen::VectorXf> pred_state);
    
    virtual void Correction(const Eigen::Ref<const Eigen::VectorXf> & pred_state, const Eigen::Ref<const Eigen::MatrixXf> & measurements, Eigen::Ref<Eigen::VectorXf> cor_state);

};

#endif /* KALMANFILTERINGFUNCTION_H */