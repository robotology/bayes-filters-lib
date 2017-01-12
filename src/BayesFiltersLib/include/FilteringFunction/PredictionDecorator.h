#ifndef PREDICTIONDECORATOR_H
#define PREDICTIONDECORATOR_H

#include <memory>

#include <FilteringFunction/Prediction.h>


class PredictionDecorator : public Prediction {
public:
    void predict(const Eigen::Ref<const Eigen::VectorXf>& prev_state, Eigen::Ref<Eigen::VectorXf> pred_state) override;

protected:
    /* Default constructor, disabled */
    PredictionDecorator() = delete;

    /* Decorator constructor */
    PredictionDecorator(std::shared_ptr<Prediction> prediction) noexcept;

    /* Destructor */
    ~PredictionDecorator() noexcept override;

    /* Copy constructor */
    PredictionDecorator(const PredictionDecorator& prediction);

    /* Move constructor */
    PredictionDecorator(PredictionDecorator&& prediction) noexcept;

    /* Copy assignment operator */
    PredictionDecorator& operator=(const PredictionDecorator& prediction);

    /* Move assignment operator */
    PredictionDecorator& operator=(PredictionDecorator&& prediction) noexcept;

    std::shared_ptr<Prediction> prediction_;
};

#endif /* PREDICTIONDECORATOR_H */
