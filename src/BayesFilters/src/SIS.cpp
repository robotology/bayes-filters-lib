#include <BayesFilters/SIS.h>

#include <fstream>
#include <iostream>
#include <utility>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;


SIS::SIS(unsigned int num_particle) noexcept :
    num_particle_(num_particle)
{ }


SIS::~SIS() noexcept
{ }


SIS::SIS(SIS&& sir_pf) noexcept :
    ParticleFilter(std::move(sir_pf))
{
    num_particle_ = sir_pf.num_particle_;
}


SIS& SIS::operator=(SIS&& sir_pf) noexcept
{
    ParticleFilter::operator=(std::move(sir_pf));

    return *this;
}


bool SIS::initialization()
{
    pred_particle_ = MatrixXf(4, num_particle_);
    pred_weight_ = MatrixXf(num_particle_, 1);

    cor_particle_ = MatrixXf(4, num_particle_);
    cor_weight_ = MatrixXf(num_particle_, 1);

    return initialization_->initialize(pred_particle_, pred_weight_);
}


void SIS::filteringStep()
{
    if (getFilteringStep() != 0)
        prediction_->predict(cor_particle_, cor_weight_,
                             pred_particle_, pred_weight_);

    correction_->correct(pred_particle_, pred_weight_,
                         cor_particle_, cor_weight_);

    cor_weight_ /= cor_weight_.sum();


    logger(pred_particle_.transpose(), pred_weight_.transpose(), cor_particle_.transpose(), cor_weight_.transpose());


    if (resampling_->neff(cor_weight_) < static_cast<float>(num_particle_)/3.0)
    {
        MatrixXf res_particle(4, num_particle_);
        VectorXf res_weight(num_particle_, 1);
        VectorXf res_parent(num_particle_, 1);

        resampling_->resample(cor_particle_, cor_weight_,
                              res_particle, res_weight, res_parent);

        cor_particle_ = res_particle;
        cor_weight_   = res_weight;
    }
}


bool SIS::runCondition()
{
    return true;
}
