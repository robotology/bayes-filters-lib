#include <BayesFilters/GaussianLikelihood.h>

using namespace bfl;
using namespace Eigen;


GaussianLikelihood::GaussianLikelihood(std::unique_ptr<PFCorrection> correction) noexcept :
    GaussianLikelihood(std::move(correction), 1.0) { }


GaussianLikelihood::GaussianLikelihood(std::unique_ptr<PFCorrection> correction, const double scale_factor) noexcept :
    PFCorrectionDecorator(std::move(correction)),
    scale_factor_(scale_factor) { }


std::pair<bool, VectorXf> GaussianLikelihood::likelihood(const Ref<const MatrixXf>& innovations)
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
            likelihood(i) = scale_factor_ * (-0.5 * static_cast<float>(innovations.rows()) * log(2.0*M_PI) - 0.5 * log(covariance_matrix.determinant()) - 0.5 * (innovations.col(i).transpose() * covariance_matrix.inverse() * innovations.col(i)).array()).exp().cast<double>().coeff(0);

        if (valid_previous_likelihood)
            likelihood.cwiseProduct(previous_likelihood);

        return std::make_pair(true, likelihood);
    }

    return std::make_pair(false, VectorXf::Zero(1));
}
