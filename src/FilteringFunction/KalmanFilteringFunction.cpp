#include "KalmanFilteringFunction.h"

/**
 * KalmanFilteringFunction implementation
 */

using namespace Eigen;


void KalmanFilteringFunction::StateModel(const Ref<const VectorXf> & prev_state, Ref<VectorXf> prop_state) {};


Ref<VectorXf> KalmanFilteringFunction::ObservationModel(const Ref<const VectorXf> & pred_state)
{
    VectorXf m(2);
    m << pred_state(0), pred_state(3);
    return m;
};


void KalmanFilteringFunction::Prediction(const Ref<const VectorXf> & prev_state, Ref<VectorXf> pred_state) {};


void KalmanFilteringFunction::Correction(const Ref<const VectorXf> & pred_state, const Ref<const VectorXf> & measurements, Ref<VectorXf> cor_state) {};