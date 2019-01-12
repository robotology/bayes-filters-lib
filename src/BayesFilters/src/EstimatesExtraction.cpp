#include <BayesFilters/EstimatesExtraction.h>
#include <BayesFilters/directional_statistics.h>

using namespace bfl;
using namespace Eigen;


EstimatesExtraction::EstimatesExtraction(const std::size_t linear_size) noexcept :
    EstimatesExtraction(linear_size, 0)
{ }


EstimatesExtraction::EstimatesExtraction(const std::size_t linear_size, const std::size_t circular_size) noexcept :
    linear_size_(linear_size),
    circular_size_(circular_size),
    state_size_(linear_size + circular_size),
    hist_buffer_(linear_size + circular_size)
{ }


EstimatesExtraction::EstimatesExtraction(EstimatesExtraction&& estimate_extraction) noexcept :
    extraction_method_(estimate_extraction.extraction_method_),
    hist_buffer_(std::move(estimate_extraction.hist_buffer_)),
    sm_weights_(std::move(estimate_extraction.sm_weights_)),
    wm_weights_(std::move(estimate_extraction.wm_weights_)),
    em_weights_(std::move(estimate_extraction.em_weights_)),
    linear_size_(estimate_extraction.linear_size_),
    circular_size_(estimate_extraction.circular_size_),
    state_size_(estimate_extraction.state_size_)
{
    estimate_extraction.extraction_method_ = ExtractionMethod::emode;
}


EstimatesExtraction& EstimatesExtraction::operator=(EstimatesExtraction&& estimate_extraction) noexcept
{
    if (this != &estimate_extraction)
    {
        extraction_method_ = estimate_extraction.extraction_method_;
        estimate_extraction.extraction_method_ = ExtractionMethod::emode;

        hist_buffer_ = std::move(estimate_extraction.hist_buffer_);

        sm_weights_ = std::move(estimate_extraction.sm_weights_);
        wm_weights_ = std::move(estimate_extraction.wm_weights_);
        em_weights_ = std::move(estimate_extraction.em_weights_);

        linear_size_ = estimate_extraction.linear_size_;
        circular_size_ = estimate_extraction.circular_size_;
        state_size_ = estimate_extraction.state_size_;
    }

    return *this;
}


bool EstimatesExtraction::setMethod(const ExtractionMethod& extraction_method)
{
    extraction_method_ = extraction_method;

    return true;
}


bool EstimatesExtraction::setMobileAverageWindowSize(const int window)
{
    if (window > 0)
        return hist_buffer_.setHistorySize(window);
    else
        return false;
}


VectorXd EstimatesExtraction::extract(const Ref<const MatrixXd>& particles, const Ref<const VectorXd>& weights)
{
    VectorXd out_particle(state_size_);
    switch (extraction_method_)
    {
        case ExtractionMethod::mean :
            out_particle = mean(particles, weights);
            break;

        case ExtractionMethod::smean :
            out_particle = simpleAverage(particles, weights, Statistics::mean);
            break;

        case ExtractionMethod::wmean :
            out_particle = weightedAverage(particles, weights, Statistics::mean);
            break;

        case ExtractionMethod::emean :
            out_particle = exponentialAverage(particles, weights, Statistics::mean);
            break;

        case ExtractionMethod::mode :
            out_particle = mode(particles, weights);
            break;

        case ExtractionMethod::smode :
            out_particle = simpleAverage(particles, weights, Statistics::mode);
            break;

        case ExtractionMethod::wmode :
            out_particle = weightedAverage(particles, weights, Statistics::mode);
            break;

        case ExtractionMethod::emode :
            out_particle = exponentialAverage(particles, weights, Statistics::mode);
            break;
    }

    return out_particle;
}


bool EstimatesExtraction::clear()
{
    return hist_buffer_.clear();
}


std::vector<std::string> EstimatesExtraction::getInfo() const
{
    std::vector<std::string> info;

    info.push_back("<| Current window size: " + std::to_string(hist_buffer_.getHistorySize()) + " |>");
    info.push_back("<| Available estimate extraction methods: " +
                   std::string(extraction_method_ == ExtractionMethod::mean  ? "1) mean <-- In use; "  : "1) mean; " ) +
                   std::string(extraction_method_ == ExtractionMethod::smean ? "2) smean <-- In use; " : "2) smean; ") +
                   std::string(extraction_method_ == ExtractionMethod::wmean ? "3) wmean <-- In use; " : "3) wmean; ") +
                   std::string(extraction_method_ == ExtractionMethod::emean ? "4) emean <-- In use; " : "4) emean; ") +
                   std::string(extraction_method_ == ExtractionMethod::mode  ? "5) mode <-- In use; "  : "5) mode; " ) +
                   std::string(extraction_method_ == ExtractionMethod::smode ? "6) smode <-- In use; " : "6) smode; ") +
                   std::string(extraction_method_ == ExtractionMethod::wmode ? "7) wmode <-- In use; " : "7) wmode; ") +
                   std::string(extraction_method_ == ExtractionMethod::emode ? "8) emode <-- In use; " : "8) emode"  ) + " |>");

    return info;
}


VectorXd EstimatesExtraction::mean(const Ref<const MatrixXd>& particles, const Ref<const VectorXd>& weights) const
{
    VectorXd out_particle(state_size_);

    if (linear_size_ > 0)
        out_particle.head(linear_size_) = particles.topRows(linear_size_) * weights;

    if (circular_size_ > 0)
        out_particle.tail(circular_size_) = directional_statistics::directional_mean(particles.bottomRows(circular_size_), weights);

    return out_particle;
}


VectorXd EstimatesExtraction::mode(const Ref<const MatrixXd>& particles, const Ref<const VectorXd>& weights) const
{
    MatrixXd::Index maxRow;
    MatrixXd::Index maxCol;
    weights.maxCoeff(&maxRow, &maxCol);

    return particles.col(maxRow);
}


VectorXd EstimatesExtraction::simpleAverage(const Ref<const MatrixXd>& particles, const Ref<const VectorXd>& weights,
                                            const Statistics& base_est_ext)
{
    VectorXd cur_estimates;
    if (base_est_ext == Statistics::mean)
        cur_estimates = mean(particles, weights);
    else if (base_est_ext == Statistics::mode)
        cur_estimates = mode(particles, weights);


    hist_buffer_.addElement(cur_estimates);

    MatrixXd history = hist_buffer_.getHistoryBuffer();
    if (sm_weights_.size() != history.cols())
        sm_weights_ = VectorXd::Ones(history.cols()) / history.cols();


    return mean(history, sm_weights_);
}


VectorXd EstimatesExtraction::weightedAverage(const Ref<const MatrixXd>& particles, const Ref<const VectorXd>& weights,
                                              const Statistics& base_est_ext)
{
    VectorXd cur_estimates;
    if (base_est_ext == Statistics::mean)
        cur_estimates = mean(particles, weights);
    else if (base_est_ext == Statistics::mode)
        cur_estimates = mode(particles, weights);


    hist_buffer_.addElement(cur_estimates);

    MatrixXd history = hist_buffer_.getHistoryBuffer();
    if (wm_weights_.size() != history.cols())
    {
        wm_weights_.resize(history.cols());
        for (unsigned int i = 0; i < history.cols(); ++i)
            wm_weights_(i) = history.cols() - i;

        wm_weights_ /= wm_weights_.sum();
    }


    return mean(history, wm_weights_);
}


VectorXd EstimatesExtraction::exponentialAverage(const Ref<const MatrixXd>& particles, const Ref<const VectorXd>& weights,
                                                 const Statistics& base_est_ext)
{
    VectorXd cur_estimates;
    if (base_est_ext == Statistics::mean)
        cur_estimates = mean(particles, weights);
    else if (base_est_ext == Statistics::mode)
        cur_estimates = mode(particles, weights);


    hist_buffer_.addElement(cur_estimates);

    MatrixXd history = hist_buffer_.getHistoryBuffer();
    if (em_weights_.size() != history.cols())
    {
        em_weights_.resize(history.cols());
        for (unsigned int i = 0; i < history.cols(); ++i)
            em_weights_(i) = std::exp(-(static_cast<double>(i) / history.cols()));

        em_weights_ /= em_weights_.sum();
    }

    return mean(history, em_weights_);
}
