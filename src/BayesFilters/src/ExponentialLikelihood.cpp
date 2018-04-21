#include <BayesFilters/ExponentialLikelihood.h>

using namespace bfl;
using namespace Eigen;


ExponentialLikelihood::ExponentialLikelihood(std::unique_ptr<PFCorrection> correction) noexcept :
    PFCorrectionDecorator(std::move(correction)){ }


std::pair<bool, VectorXf> ExponentialLikelihood::likelihood(const Ref<const MatrixXf>& innovations)
{
    bool valid_previous_likelihood;
    VectorXf previous_likelihood;
    std::tie(valid_previous_likelihood, previous_likelihood) = PFCorrectionDecorator::likelihood(innovations);

    VectorXf likelihood(innovations.cols());
    bool valid_covariance_matrix;
    MatrixXf covariance_matrix;
    std::tie(valid_covariance_matrix, covariance_matrix) = measurement_model_->getNoiseCovarianceMatrix();

    if (valid_covariance_matrix)
    {
        for (unsigned int i = 0; i < innovations.cols(); ++i)
            likelihood(i) = (-0.5 * static_cast<float>(innovations.rows()) * log(2.0*M_PI) - 0.5 * log(covariance_matrix.determinant()) - 0.5 * (innovations.col(i).transpose() * covariance_matrix.inverse() * innovations.col(i)).array()).exp().cast<double>().coeff(0);

        if (valid_previous_likelihood)
            likelihood.cwiseProduct(previous_likelihood);

        return std::make_pair(true, likelihood);
    }

    return std::make_pair(false, VectorXf::Zero(1));
}
