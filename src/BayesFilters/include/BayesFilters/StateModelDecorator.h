#ifndef STATEMODELDECORATOR_H
#define STATEMODELDECORATOR_H

#include <BayesFilters/StateModel.h>

#include <memory>

namespace bfl {
    class StateModelDecorator;
}


class bfl::StateModelDecorator : public StateModel
{
public:
    void propagate(const Eigen::Ref<const Eigen::MatrixXd>& cur_states, Eigen::Ref<Eigen::MatrixXd> prop_states) override;

    void motion(const Eigen::Ref<const Eigen::MatrixXd>& cur_states, Eigen::Ref<Eigen::MatrixXd> mot_states) override;

    bool setProperty(const std::string& property) override;

    Eigen::MatrixXd getNoiseCovarianceMatrix() override;

    Eigen::MatrixXd getNoiseSample(const std::size_t num) override;

protected:
    StateModelDecorator(std::unique_ptr<StateModel> state_model) noexcept;

    StateModelDecorator(StateModelDecorator&& state_model) noexcept;

    virtual ~StateModelDecorator() noexcept;

    StateModelDecorator& operator=(StateModelDecorator&& state_model) noexcept;

private:
    std::unique_ptr<StateModel> state_model_;
};

#endif /* STATEMODELDECORATOR_H */
