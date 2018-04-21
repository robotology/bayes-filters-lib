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


SIS::~SIS() noexcept { }


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


void SIS::initialization()
{
    pred_particle_.resize(4, num_particle_);
    pred_weight_.resize(num_particle_, 1);

    cor_particle_.resize(4, num_particle_);
    cor_weight_.resize(num_particle_, 1);

    initialization_->initialize(pred_particle_, pred_weight_);

    result_pred_particle_.resize(simulation_time_);
    result_pred_weight_.resize(simulation_time_);

    result_cor_particle_.resize(simulation_time_);
    result_cor_weight_.resize(simulation_time_);
}


void SIS::filteringStep()
{
    unsigned int k = getFilteringStep();

    if (k != 0)
        prediction_->predict(cor_particle_, cor_weight_,
                             pred_particle_, pred_weight_);

    correction_->correct(pred_particle_, pred_weight_,
                         cor_particle_, cor_weight_);

    cor_weight_ /= cor_weight_.sum();


    /* Here results should be saves. */
    /* Proper strategy is WIP. */
    result_pred_particle_[k] = pred_particle_;
    result_pred_weight_  [k] = pred_weight_;

    result_cor_particle_[k]  = cor_particle_;
    result_cor_weight_  [k]  = cor_weight_;


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


void SIS::getResult()
{
    std::ofstream result_file_object;
    std::ofstream result_file_measurement;
    std::ofstream result_file_pred_particle;
    std::ofstream result_file_pred_weight;
    std::ofstream result_file_cor_particle;
    std::ofstream result_file_cor_weight;

    result_file_object.open       ("./result_object.txt");
    result_file_measurement.open  ("./result_measurement.txt");
    result_file_pred_particle.open("./result_pred_particle.txt");
    result_file_pred_weight.open  ("./result_pred_weight.txt");
    result_file_cor_particle.open ("./result_cor_particle.txt");
    result_file_cor_weight.open   ("./result_cor_weight.txt");

    result_file_object       << object_;
    result_file_measurement  << measurement_;
    for (unsigned int k = 0; k < getFilteringStep(); ++k)
    {
        result_file_pred_particle << result_pred_particle_[k] << std::endl << std::endl;
        result_file_pred_weight   << result_pred_weight_[k]   << std::endl << std::endl;

        result_file_cor_particle  << result_cor_particle_[k]  << std::endl << std::endl;
        result_file_cor_weight    << result_cor_weight_[k]    << std::endl << std::endl;
    }

    result_file_object.close();
    result_file_measurement.close();
    result_file_pred_particle.close();
    result_file_pred_weight.close();
    result_file_cor_particle.close();
    result_file_cor_weight.close();
}
