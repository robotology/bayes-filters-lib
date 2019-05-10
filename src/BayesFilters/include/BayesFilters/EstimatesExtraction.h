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
    EstimatesExtraction(const std::size_t linear_size) noexcept;

    EstimatesExtraction(const std::size_t linear_size, const std::size_t circular_size) noexcept;

    EstimatesExtraction(EstimatesExtraction&& estimate_extraction) noexcept;

    EstimatesExtraction& operator=(EstimatesExtraction&& estimate_extraction) noexcept;

    ~EstimatesExtraction() noexcept =default;

    enum class ExtractionMethod
    {
        mean, smean, wmean, emean,
        mode, smode, wmode, emode,
        map, smap, wmap, emap
    };

    bool setMethod(const ExtractionMethod& extraction_method);

    bool setMobileAverageWindowSize(const int window);

    std::pair<bool, Eigen::VectorXd> extract(const Eigen::Ref<const Eigen::MatrixXd>& particles, const Eigen::Ref<const Eigen::VectorXd>& weights);

    std::pair<bool, Eigen::VectorXd> extract(const Eigen::Ref<const Eigen::MatrixXd>& particles, const Eigen::Ref<const Eigen::VectorXd>& weights, const Eigen::Ref<const Eigen::VectorXd>& previous_weights, const Eigen::Ref<const Eigen::VectorXd>& likelihoods, const Eigen::Ref<const Eigen::MatrixXd>& transition_probabilities);

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
        mode,
        map
    };

    Eigen::VectorXd mean(const Eigen::Ref<const Eigen::MatrixXd>& particles, const Eigen::Ref<const Eigen::VectorXd>& weights) const;

    Eigen::VectorXd mode(const Eigen::Ref<const Eigen::MatrixXd>& particles, const Eigen::Ref<const Eigen::VectorXd>& weights) const;

    /**
     * Return an approximatation of the MAP (maximum a posteriori) estimate from a running particle filter.
     * The approximation is taken from:
     * Saha, S., Boers, Y., Driessen, H., Mandal, P. K., Bagchi, A. (2009),
     * 'Particle Based MAP State Estimation: A Comparison.',
     * 12th International Conference on Information Fusion,
     * Seattle, WA, USA, July 6-9, 2009
     *
     * The vector 'particles' contains the position of the particles at the current time step k.
     * The vector 'previous_weights' contains the weights at the previous time step (k - 1).
     * The vector 'likelihoods' containts the likelihoods at the current time step k.
     * The matrix 'transition_probabilities' contains, in each entry (i, j), the transition probability
     * between the i-th particle at time k and the j-th particle at time (k-1).
     *
     */
    Eigen::VectorXd map(const Eigen::Ref<const Eigen::MatrixXd>& particles, const Eigen::Ref<const Eigen::VectorXd>& previous_weights, const Eigen::Ref<const Eigen::VectorXd>& likelihoods, const Eigen::Ref<const Eigen::MatrixXd>& transition_probabilities) const;

    Eigen::VectorXd simpleAverage(const Eigen::Ref<const Eigen::MatrixXd>& particles, const Eigen::Ref<const Eigen::VectorXd>& weights, const Eigen::Ref<const Eigen::VectorXd>& previous_weights, const Eigen::Ref<const Eigen::VectorXd>& likelihoods, const Eigen::Ref<const Eigen::MatrixXd>& transition_probabilities, const Statistics& base_est_ext);

    Eigen::VectorXd weightedAverage(const Eigen::Ref<const Eigen::MatrixXd>& particles, const Eigen::Ref<const Eigen::VectorXd>& weights, const Eigen::Ref<const Eigen::VectorXd>& previous_weights, const Eigen::Ref<const Eigen::VectorXd>& likelihoods, const Eigen::Ref<const Eigen::MatrixXd>& transition_probabilities, const Statistics& base_est_ext);

    Eigen::VectorXd exponentialAverage(const Eigen::Ref<const Eigen::MatrixXd>& particles, const Eigen::Ref<const Eigen::VectorXd>& weights, const Eigen::Ref<const Eigen::VectorXd>& previous_weights, const Eigen::Ref<const Eigen::VectorXd>& likelihoods, const Eigen::Ref<const Eigen::MatrixXd>& transition_probabilities, const Statistics& base_est_ext);

    std::size_t linear_size_;

    std::size_t circular_size_;

    std::size_t state_size_;
};

#endif /* ESTIMATESEXTRACTION_H */
