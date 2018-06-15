#include <BayesFilters/GaussianLikelihood.h>

using namespace bfl;
using namespace Eigen;


GaussianLikelihood::GaussianLikelihood() noexcept :
    GaussianLikelihood(1.0) { }


GaussianLikelihood::GaussianLikelihood(const double scale_factor) noexcept :
    scale_factor_(scale_factor) { }


std::pair<bool, VectorXf> GaussianLikelihood::likelihood(const MeasurementModel& measurement_model, const Ref<const MatrixXf>& pred_states)
{
    bool valid_measurements;
    MatrixXf measurements;
    std::tie(valid_measurements, measurements) = measurement_model.getProcessMeasurements();

    if (!valid_measurements)
        return std::make_pair(false, VectorXf::Zero(1));


    bool valid_predicted_measurements;
    MatrixXf predicted_measurements;
    std::tie(valid_predicted_measurements, predicted_measurements) = measurement_model.predictedMeasure(pred_states);

    if (!valid_predicted_measurements)
        return std::make_pair(false, VectorXf::Zero(1));


    bool valid_innovation;
    MatrixXf innovations;
    std::tie(valid_innovation, innovations) = measurement_model.innovation(predicted_measurements, measurements);

    if (!valid_innovation)
        return std::make_pair(false, VectorXf::Zero(1));


    VectorXf likelihood(innovations.cols());
    bool valid_covariance_matrix;
    MatrixXf covariance_matrix;
    std::tie(valid_covariance_matrix, covariance_matrix) = measurement_model.getNoiseCovarianceMatrix();

    if (!valid_covariance_matrix)
        return std::make_pair(false, VectorXf::Zero(1));


    for (unsigned int i = 0; i < innovations.cols(); ++i)
        likelihood(i) = scale_factor_ * (-0.5 * static_cast<float>(innovations.rows()) * log(2.0*M_PI) - 0.5 * log(covariance_matrix.determinant()) - 0.5 * (innovations.col(i).transpose() * covariance_matrix.inverse() * innovations.col(i)).array()).exp().cast<double>().coeff(0);

    return std::make_pair(true, likelihood);
}
