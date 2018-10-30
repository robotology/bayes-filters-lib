#ifndef LINEARMODEL_H
#define LINEARMODEL_H

#include <BayesFilters/MeasurementModel.h>

#include <fstream>
#include <functional>
#include <random>
#include <string>

namespace bfl {
    class LinearModel;
}


class bfl::LinearModel : public MeasurementModel
{
public:
    LinearModel(const float sigma_x, const float sigma_y, const unsigned int seed) noexcept;

    LinearModel(const float sigma_x, const float sigma_y) noexcept;

    LinearModel() noexcept;

    LinearModel(const LinearModel& lin_sense);

    LinearModel(LinearModel&& lin_sense) noexcept;

    virtual ~LinearModel() noexcept;

    LinearModel& operator=(const LinearModel& lin_sense) noexcept;

    LinearModel& operator=(LinearModel&& lin_sense) noexcept;

    std::pair<bool, bfl::Data> measure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) const override;

    std::pair<bool, bfl::Data> predictedMeasure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) const override;

    std::pair<bool, bfl::Data> innovation(const bfl::Data& predicted_measurements, const bfl::Data& measurements) const override;

    std::pair<bool, Eigen::MatrixXf> getNoiseSample(const int num) const;

    std::pair<bool, Eigen::MatrixXf> getNoiseCovarianceMatrix() const override;

private:
    std::mt19937_64 generator_;

    std::normal_distribution<float> distribution_;

    bool log_enabled_ = false;

    mutable std::ofstream log_file_measurements_;

protected:
    /**
     * The Sampling interval in [time].
     */
    float T_;

    /**
     * x-axis measurement noise std deviation in [length].
     */
    float sigma_x_;

    /**
     * y-axis measurement noise std deviation in [length].
     */
    float sigma_y_;

    /**
     * Measurement matrix.
     */
    Eigen::MatrixXf H_;

    /**
     * Convariance matrix of the additive white noise of the measurements.
     */
    Eigen::Matrix2f R_;

    /**
     * Square root matrix of R_.
     */
    Eigen::Matrix2f sqrt_R_;

    /**
     * Random number generator function from a Normal distribution.
     * A call to `gauss_rnd_sample_()` returns a floating point random number.
     */
    std::function<float()> gauss_rnd_sample_;

    std::vector<std::string> log_filenames(const std::string& prefix_path, const std::string& prefix_name) override
    {
        return {prefix_path + "/" + prefix_name + "_measurements"};
    }
};

#endif /* LINEARMODEL_H */
