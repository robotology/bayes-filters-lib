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
{
    if (log_enabled_)
        disableLog();
}


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


    if (log_enabled_)
        logger(pred_particle_, pred_weight_, cor_particle_, cor_weight_);


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


void SIS::enableLog(const std::string& prefix_name)
{
    prefix_name_ = prefix_name;

    log_file_pred_particle_.open("./" + prefix_name_ + "_pred_particle.txt", std::ofstream::out | std::ofstream::app);
    log_file_pred_weight_.open  ("./" + prefix_name_ + "_pred_weight.txt",   std::ofstream::out | std::ofstream::app);
    log_file_cor_particle_.open ("./" + prefix_name_ + "_cor_particle.txt",  std::ofstream::out | std::ofstream::app);
    log_file_cor_weight_.open   ("./" + prefix_name_ + "_cor_weight.txt",    std::ofstream::out | std::ofstream::app);

    log_enabled_ = true;
}


void SIS::disableLog()
{
    log_enabled_ = false;

    log_file_pred_particle_.close();
    log_file_pred_weight_.close();
    log_file_cor_particle_.close();
    log_file_cor_weight_.close();
}


void SIS::logger(const Ref<const MatrixXf>& pred_particle, const Ref<const VectorXf>& pred_weight,
              const Ref<const MatrixXf>& cor_particle,  const Ref<const VectorXf>& cor_weight) const
{
    log_file_pred_particle_ << pred_particle.transpose() << std::endl;
    log_file_pred_weight_ << pred_weight.transpose() << std::endl;

    log_file_cor_particle_ << cor_particle.transpose() << std::endl;
    log_file_cor_weight_ << cor_weight.transpose() << std::endl;
}