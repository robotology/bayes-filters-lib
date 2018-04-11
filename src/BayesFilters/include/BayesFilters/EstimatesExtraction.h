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

    Eigen::VectorXf extract(const Eigen::Ref<const Eigen::MatrixXf>& particles, const Eigen::Ref<const Eigen::VectorXf>& weights);

    bool clear();


    std::vector<std::string> getInfo() const;

protected:
    ExtractionMethod extraction_method_ = ExtractionMethod::emode;

    HistoryBuffer hist_buffer_;

    Eigen::VectorXf sm_weights_;
    Eigen::VectorXf wm_weights_;
    Eigen::VectorXf em_weights_;


    enum class Statistics
    {
        mean,
        mode
    };

    Eigen::VectorXf mean(const Eigen::Ref<const Eigen::MatrixXf>& particles, const Eigen::Ref<const Eigen::VectorXf>& weights) const;

    Eigen::VectorXf mode(const Eigen::Ref<const Eigen::MatrixXf>& particles, const Eigen::Ref<const Eigen::VectorXf>& weights) const;

    Eigen::VectorXf simpleAverage(const Eigen::Ref<const Eigen::MatrixXf>& particles, const Eigen::Ref<const Eigen::VectorXf>& weights,
                                  const Statistics& base_est_ext);

    Eigen::VectorXf weightedAverage(const Eigen::Ref<const Eigen::MatrixXf>& particles, const Eigen::Ref<const Eigen::VectorXf>& weights,
                                    const Statistics& base_est_ext);

    Eigen::VectorXf exponentialAverage(const Eigen::Ref<const Eigen::MatrixXf>& particles, const Eigen::Ref<const Eigen::VectorXf>& weights,
                                       const Statistics& base_est_ext);
};

#endif /* ESTIMATESEXTRACTION_H */
