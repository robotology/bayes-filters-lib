#include <BayesFilters/SIS.h>
#include <BayesFilters/utils.h>

#include <fstream>
#include <iostream>
#include <utility>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


SIS::SIS
(
    unsigned int num_particle,
    std::size_t state_size_linear,
    std::size_t state_size_circular
) noexcept :
    num_particle_(num_particle),
    state_size_(state_size_linear + state_size_circular),
    pred_particle_(num_particle_, state_size_linear, state_size_circular),
    cor_particle_(num_particle_, state_size_linear, state_size_circular)
{ }


SIS::SIS(unsigned int num_particle, std::size_t state_size_linear) noexcept :
    SIS(num_particle, state_size_linear, 0)
{ }


SIS::~SIS() noexcept
{ }


SIS::SIS(SIS&& sir_pf) noexcept :
    ParticleFilter(std::move(sir_pf)),
    pred_particle_(std::move(sir_pf.pred_particle_)),
    cor_particle_(std::move(sir_pf.cor_particle_)),
    num_particle_(sir_pf.num_particle_),
    state_size_(sir_pf.state_size_) { }


SIS& SIS::operator=(SIS&& sir_pf) noexcept
{
    ParticleFilter::operator=(std::move(sir_pf));

    num_particle_ = sir_pf.num_particle_;

    state_size_ = sir_pf.state_size_;

    pred_particle_ = std::move(sir_pf.pred_particle_);

    cor_particle_ = std::move(sir_pf.cor_particle_);

    return *this;
}


bool SIS::initialization()
{
    return initialization_->initialize(pred_particle_);
}


void SIS::filteringStep()
{
    if (getFilteringStep() != 0)
        prediction_->predict(cor_particle_, pred_particle_);

    correction_->correct(pred_particle_, cor_particle_);

    /* Normalize weights using LogSumExp. */
    cor_particle_.weight().array() -= utils::log_sum_exp(cor_particle_.weight());

    logger(pred_particle_.state().transpose(), pred_particle_.weight().transpose(),
           cor_particle_.state().transpose(), cor_particle_.weight().transpose());


    if (resampling_->neff(cor_particle_.weight()) < static_cast<double>(num_particle_)/3.0)
    {
        ParticleSet res_particle(num_particle_, state_size_);
        VectorXi res_parent(num_particle_, 1);

        resampling_->resample(cor_particle_, res_particle, res_parent);

        cor_particle_ = res_particle;
    }
}


bool SIS::runCondition()
{
    return true;
}
