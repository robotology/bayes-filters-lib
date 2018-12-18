#ifndef PFPREDICTION_H
#define PFPREDICTION_H

#include <BayesFilters/ExogenousModel.h>
#include <BayesFilters/ParticleSet.h>
#include <BayesFilters/StateModel.h>

#include <Eigen/Dense>
#include <memory>
#include <string>

namespace bfl {
    class PFPrediction;
}


class bfl::PFPrediction
{
public:
    virtual ~PFPrediction() noexcept { };

    void predict(const bfl::ParticleSet& prev_particles, bfl::ParticleSet& pred_particles);

    bool skip(const std::string& what_step, const bool status);

    bool getSkipState();

    bool getSkipExogenous();

    virtual void setStateModel(std::unique_ptr<StateModel> state_model) = 0;

    virtual void setExogenousModel(std::unique_ptr<ExogenousModel> exogenous_model);

    /* FIXME
     * This function calls may be deleted in future releases. */
    virtual StateModel& getStateModel() = 0;

    /* FIXME
     * This function calls may be deleted in future releases. */
    virtual ExogenousModel& getExogenousModel();

protected:
    PFPrediction() noexcept;

    PFPrediction(PFPrediction&& pf_prediction) noexcept;

    virtual void predictStep(const bfl::ParticleSet& prev_particles, bfl::ParticleSet& pred_particles) = 0;

private:
    bool skip_prediction_ = false;

    bool skip_state_      = false;

    bool skip_exogenous_  = false;

    friend class PFPredictionDecorator;
};

#endif /* PFPREDICTION_H */
