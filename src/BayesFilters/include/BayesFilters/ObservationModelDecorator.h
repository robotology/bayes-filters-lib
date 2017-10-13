#ifndef OBSERVATIONMODELDECORATOR_H
#define OBSERVATIONMODELDECORATOR_H

#include "ObservationModel.h"

#include <memory>

namespace bfl {
    class ObservationModelDecorator;
}


class bfl::ObservationModelDecorator : public ObservationModel
{
public:
    void observe(const Eigen::Ref<const Eigen::MatrixXf>& cur_states, Eigen::Ref<Eigen::MatrixXf> observations) override;

    void measure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states, Eigen::Ref<Eigen::MatrixXf> measurements) override;

    Eigen::MatrixXf getNoiseSample(const int num) override;

    Eigen::MatrixXf getNoiseCovariance() override;

    bool setProperty(const std::string property) override;

protected:
    ObservationModelDecorator(std::unique_ptr<ObservationModel> observation_model) noexcept;

    ObservationModelDecorator(ObservationModelDecorator&& observation_model) noexcept;

    ~ObservationModelDecorator() noexcept;

    ObservationModelDecorator& operator=(ObservationModelDecorator&& observation_model) noexcept;

private:
    std::unique_ptr<ObservationModel> observation_model_;
};

#endif /* OBSERVATIONMODELDECORATOR_H */
