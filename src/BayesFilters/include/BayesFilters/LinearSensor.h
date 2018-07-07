#ifndef LINEARSENSOR_H
#define LINEARSENSOR_H

#include <BayesFilters/MeasurementModel.h>

#include <fstream>
#include <functional>
#include <random>
#include <string>

namespace bfl {
    class LinearSensor;
}


class bfl::LinearSensor : public MeasurementModel
{
public:
    LinearSensor(const float sigma_x, const float sigma_y, const unsigned int seed) noexcept;

    LinearSensor(const float sigma_x, const float sigma_y) noexcept;

    LinearSensor() noexcept;

    LinearSensor(const LinearSensor& lin_sense);

    LinearSensor(LinearSensor&& lin_sense) noexcept;

    virtual ~LinearSensor() noexcept;

    LinearSensor& operator=(const LinearSensor& lin_sense) noexcept;

    LinearSensor& operator=(LinearSensor&& lin_sense) noexcept;

    std::pair<bool, Eigen::MatrixXf> measure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) const override;

    std::pair<bool, Eigen::MatrixXf> innovation(const Eigen::Ref<const Eigen::MatrixXf>& predicted_measurements, const Eigen::Ref<const Eigen::MatrixXf>& measurements) const override;

    std::pair<bool, Eigen::MatrixXf> predictedMeasure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states) const override;

    std::pair<bool, Eigen::MatrixXf> getNoiseSample(const int num) const override;

    std::pair<bool, Eigen::MatrixXf> getNoiseCovarianceMatrix() const override;

    bool setProperty(const std::string property) override { return false; };

    bool registerProcessData(std::shared_ptr<GenericData> process_data) override;

    std::pair<bool, Eigen::MatrixXf> getProcessMeasurements() const override;

    void enableLog(const std::string& prefix_name) override;

    void disableLog() override;

private:
    std::mt19937_64 generator_;

    std::normal_distribution<float> distribution_;

    bool log_enabled_ = false;

    std::string prefix_name_;

    std::shared_ptr<Eigen::VectorXf> process_data_;

    mutable std::ofstream log_file_measurements_;

    void logger(const Eigen::Ref<const Eigen::MatrixXf>& data) const;

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
};

#endif /* LINEARSENSOR_H */
