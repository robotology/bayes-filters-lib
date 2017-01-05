#include <FilteringFunction/KalmanFilteringFunction.h>

#include <Eigen/Dense>

using namespace Eigen;

/**
 * KalmanFilteringFunction implementation
 */


void KalmanFilteringFunction::StateModel(const Ref<const VectorXf> & prev_state, Ref<VectorXf> prop_state) {};


Ref<MatrixXf> KalmanFilteringFunction::ObservationModel(const Ref<const VectorXf> & pred_state)
{
    VectorXf m(2);
    return m;
}


void KalmanFilteringFunction::Prediction(const Ref<const VectorXf> & prev_state, Ref<VectorXf> pred_state) {};


void KalmanFilteringFunction::Correction(const Ref<const VectorXf> & pred_state, const Ref<const MatrixXf> & measurements, Ref<VectorXf> cor_state) {};
