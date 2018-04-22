#ifndef GAUSSIANLIKELIHOOD_H
#define GAUSSIANLIKELIHOOD_H

#include <BayesFilters/PFCorrectionDecorator.h>

#include <memory>

namespace bfl {
    class GaussianLikelihood;
}


class bfl::GaussianLikelihood : public PFCorrectionDecorator
{
public:
    GaussianLikelihood(std::unique_ptr<PFCorrection> correction) noexcept;

    GaussianLikelihood(std::unique_ptr<PFCorrection> correction, const double scale_factor) noexcept;

    virtual ~GaussianLikelihood() noexcept { };

protected:
    std::pair<bool, Eigen::VectorXf> likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations) override;

    double scale_factor_;
};

#endif /* GAUSSIANLIKELIHOOD_H */
