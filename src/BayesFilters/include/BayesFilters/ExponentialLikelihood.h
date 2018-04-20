#ifndef EXPONENTIALLIKELIHOOD_H
#define EXPONENTIALLIKELIHOOD_H

#include <BayesFilters/PFCorrectionDecorator.h>

#include <memory>

namespace bfl {
    class ExponentialLikelihood;
}


class bfl::ExponentialLikelihood : public PFCorrectionDecorator
{
public:
    ExponentialLikelihood(std::unique_ptr<PFCorrection> correction) noexcept;

    virtual ~ExponentialLikelihood() noexcept;

protected:
    std::pair<bool, Eigen::VectorXf> likelihood(const Eigen::Ref<const Eigen::MatrixXf>& innovations) override;
};

#endif /* PFCORRECTIONDECORATOR_H */
