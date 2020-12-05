/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef GAUSSIANFILTER_H
#define GAUSSIANFILTER_H

#include <BayesFilters/FilteringAlgorithm.h>
#include <BayesFilters/Gaussian.h>
#include <BayesFilters/GaussianPrediction.h>
#include <BayesFilters/GaussianCorrection.h>

namespace bfl {
    class GaussianFilter;
}


class bfl::GaussianFilter: public bfl::FilteringAlgorithm
{
public:
    bool skip(const std::string& what_step, const bool status) override;

protected:
    GaussianFilter(std::unique_ptr<GaussianPrediction> prediction, std::unique_ptr<GaussianCorrection> correction) noexcept;

    GaussianFilter(GaussianFilter&& kf) noexcept;

    GaussianFilter& operator=(GaussianFilter&& gf) noexcept;

    virtual ~GaussianFilter() noexcept = default;

    std::unique_ptr<GaussianPrediction> prediction_;

    std::unique_ptr<GaussianCorrection> correction_;
};

#endif /* GAUSSIANFILTER_H */
