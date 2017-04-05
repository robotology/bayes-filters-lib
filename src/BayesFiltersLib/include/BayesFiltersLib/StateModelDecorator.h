#ifndef STATEMODELDECORATOR_H
#define STATEMODELDECORATOR_H

#include "StateModel.h"

#include <memory>

namespace bfl
{
    class StateModelDecorator;
}


class bfl::StateModelDecorator : public StateModel
{
public:
    void propagate(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> prop_state) override;

    void noiseSample(Eigen::Ref<Eigen::VectorXf> sample) override;

    bool setProperty(const std::string& property) override;

protected:
    /* Default constructor, disabled */
    StateModelDecorator() = delete;

    /* Decorator constructor */
    StateModelDecorator(std::unique_ptr<StateModel> state_model) noexcept;

    /* Destructor */
    ~StateModelDecorator() noexcept override;

    /* Move constructor */
    StateModelDecorator(StateModelDecorator&& state_model) noexcept;

    /* Move assignment operator */
    StateModelDecorator& operator=(StateModelDecorator&& state_model) noexcept;

private:
    std::unique_ptr<StateModel> state_model_;
};

#endif /* STATEMODELDECORATOR_H */
