/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef ESTIMATESEXTRACTION_H
#define ESTIMATESEXTRACTION_H

#include <string>
#include <vector>

#include <Eigen/Core>

#include <BayesFilters/HistoryBuffer.h>

namespace bfl {
    class EstimatesExtraction;
}


class bfl::EstimatesExtraction
{
public:
    EstimatesExtraction() noexcept { };

    EstimatesExtraction(EstimatesExtraction&& estimate_extraction) noexcept;

    EstimatesExtraction& operator=(EstimatesExtraction&& estimate_extraction) noexcept;

    ~EstimatesExtraction() noexcept { };


    enum class ExtractionMethod
    {
        mean,
        smean,
        wmean,
        emean,
        mode,
        smode,
        wmode,
        emode
    };


    bool setMethod(const ExtractionMethod& extraction_method);

    bool setMobileAverageWindowSize(const int window);

    Eigen::VectorXd extract(const Eigen::Ref<const Eigen::MatrixXd>& particles, const Eigen::Ref<const Eigen::VectorXd>& weights);

    bool clear();


    std::vector<std::string> getInfo() const;

protected:
    ExtractionMethod extraction_method_ = ExtractionMethod::emode;

    HistoryBuffer hist_buffer_;

    Eigen::VectorXd sm_weights_;
    Eigen::VectorXd wm_weights_;
    Eigen::VectorXd em_weights_;


    enum class Statistics
    {
        mean,
        mode
    };

    Eigen::VectorXd mean(const Eigen::Ref<const Eigen::MatrixXd>& particles, const Eigen::Ref<const Eigen::VectorXd>& weights) const;

    Eigen::VectorXd mode(const Eigen::Ref<const Eigen::MatrixXd>& particles, const Eigen::Ref<const Eigen::VectorXd>& weights) const;

    Eigen::VectorXd simpleAverage(const Eigen::Ref<const Eigen::MatrixXd>& particles, const Eigen::Ref<const Eigen::VectorXd>& weights,
                                  const Statistics& base_est_ext);

    Eigen::VectorXd weightedAverage(const Eigen::Ref<const Eigen::MatrixXd>& particles, const Eigen::Ref<const Eigen::VectorXd>& weights,
                                    const Statistics& base_est_ext);

    Eigen::VectorXd exponentialAverage(const Eigen::Ref<const Eigen::MatrixXd>& particles, const Eigen::Ref<const Eigen::VectorXd>& weights,
                                       const Statistics& base_est_ext);
};

#endif /* ESTIMATESEXTRACTION_H */
