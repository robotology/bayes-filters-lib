#ifndef STATEMODELDECORATOR_H
#define STATEMODELDECORATOR_H

#include "StateModel.h"

#include <memory>

namespace bfl {
    class StateModelDecorator;
}


class bfl::StateModelDecorator : public StateModel
{
public:
    void propagate(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> prop_state) override;

    void motion(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> mot_state) override;

    Eigen::VectorXf getNoiseSample() override;

    Eigen::MatrixXf getNoiseCovariance() override;

    bool setProperty(const std::string& property) override;

protected:
    StateModelDecorator(std::unique_ptr<StateModel> state_model) noexcept;

    StateModelDecorator(StateModelDecorator&& state_model) noexcept;

    ~StateModelDecorator() noexcept override;

    StateModelDecorator& operator=(StateModelDecorator&& state_model) noexcept;

private:
    std::unique_ptr<StateModel> state_model_;
};

#endif /* STATEMODELDECORATOR_H */
