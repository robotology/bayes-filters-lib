#ifndef PREDICTIONDECORATOR_H
#define PREDICTIONDECORATOR_H

#include <memory>

#include "Prediction.h"


namespace bfl
{

class PredictionDecorator : public Prediction {
public:
    void predict(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> pred_state) override;

    bool setMotionModelProperty(const std::string& property) override;

protected:
    /* Default constructor, disabled */
    PredictionDecorator() = delete;

    /* Decorator constructor */
    PredictionDecorator(std::unique_ptr<Prediction> prediction) noexcept;

    /* Destructor */
    ~PredictionDecorator() noexcept override;

    /* Move constructor */
    PredictionDecorator(PredictionDecorator&& prediction) noexcept;

    /* Move assignment operator */
    PredictionDecorator& operator=(PredictionDecorator&& prediction) noexcept;

private:
    std::unique_ptr<Prediction> prediction_;
};

} // namespace bfl

#endif /* PREDICTIONDECORATOR_H */
