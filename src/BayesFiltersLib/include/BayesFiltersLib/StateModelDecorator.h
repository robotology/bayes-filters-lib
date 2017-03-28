#ifndef STATEMODELDECORATOR_H
#define STATEMODELDECORATOR_H

#include "StateModel.h"

#include <memory>


namespace bfl
{

class StateModelDecorator : public StateModel
{
public:
    void propagate(const Eigen::Ref<const Eigen::VectorXf>& cur_state, Eigen::Ref<Eigen::VectorXf> prop_state) override;

    void noiseSample(Eigen::Ref<Eigen::VectorXf> sample) override;

    void motion(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> next_state) override;

protected:
    /* Default constructor, disabled */
    StateModelDecorator() = delete;

    /* Decorator constructor */
    StateModelDecorator(std::unique_ptr<StateModel> prediction) noexcept;

    /* Destructor */
    ~StateModelDecorator() noexcept override;

    /* Move constructor */
    StateModelDecorator(StateModelDecorator&& prediction) noexcept;

    /* Move assignment operator */
    StateModelDecorator& operator=(StateModelDecorator&& prediction) noexcept;

private:
    std::unique_ptr<StateModel> prediction_;
};

} // namespace bfl


#endif /* STATEMODELDECORATOR_H */
