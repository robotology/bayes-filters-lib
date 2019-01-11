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

    std::size_t linear_size_;

    std::size_t circular_size_;

    std::size_t state_size_;
};

#endif /* ESTIMATESEXTRACTION_H */
