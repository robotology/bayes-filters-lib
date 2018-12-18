#ifndef GAUSSIANPREDICTION_H
#define GAUSSIANPREDICTION_H

#include <BayesFilters/GaussianMixture.h>

#include <Eigen/Dense>

namespace bfl {
    class GaussianPrediction;
}


class bfl::GaussianPrediction
{
public:
    virtual ~GaussianPrediction() noexcept { };

    void predict(const GaussianMixture& prev_state, GaussianMixture& pred_state);

    bool skip(const std::string& what_step, const bool status);

    bool getSkipState();

    bool getSkipExogenous();

protected:
    GaussianPrediction() noexcept;

    GaussianPrediction(GaussianPrediction&& g_prediction) noexcept;

    virtual void predictStep(const GaussianMixture& prev_state, GaussianMixture& pred_state) = 0;

private:
    bool skip_prediction_ = false;

    bool skip_state_      = false;

    bool skip_exogenous_  = false;

    friend class GaussianPredictionDecorator;

    friend class GPFPrediction;
};

#endif /* GAUSSIANPREDICTION_H */
