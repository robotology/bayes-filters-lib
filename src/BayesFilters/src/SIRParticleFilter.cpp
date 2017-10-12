#include "BayesFilters/SIRParticleFilter.h"

#include <fstream>
#include <iostream>
#include <utility>

#include <Eigen/Dense>

using namespace bfl;
using namespace Eigen;

    
SIRParticleFilter::SIRParticleFilter(std::unique_ptr<PFPrediction> prediction, std::unique_ptr<PFCorrection> correction, std::unique_ptr<Resampling> resampling) noexcept :
    prediction_(std::move(prediction)), correction_(std::move(correction)), resampling_(std::move(resampling)) { }


SIRParticleFilter::~SIRParticleFilter() noexcept { }


SIRParticleFilter::SIRParticleFilter(SIRParticleFilter&& sir_pf) noexcept :
    prediction_(std::move(sir_pf.prediction_)), correction_(std::move(sir_pf.correction_)), resampling_(std::move(sir_pf.resampling_)) { }


SIRParticleFilter& SIRParticleFilter::operator=(SIRParticleFilter&& sir_pf) noexcept
{
    prediction_ = std::move(sir_pf.prediction_);
    correction_ = std::move(sir_pf.correction_);
    resampling_ = std::move(sir_pf.resampling_);

    return *this;
}


void SIRParticleFilter::initialization()
{
    /* INITIALIZATION */
    simulation_time_ = 100;
    num_particle_    = 900;
    surv_x_          = 1000;
    surv_y_          = 1000;

    /* GENERATE MEASUREMENTS */
    measurement_.resize(2, simulation_time_);
    object_.resize(4, simulation_time_);

    object_.col(0) << 0, 10, 0, 10;
    correction_->getObservationModel().measure(object_.col(0), measurement_.col(0));
    for (int k = 1; k < simulation_time_; ++k)
    {
        prediction_->getStateModel().motion(object_.col(k-1), object_.col(k));
        correction_->getObservationModel().measure(object_.col(k), measurement_.col(k));
    }

    /* INITIALIZE FILTER */
    pred_particle_.resize(4, num_particle_);
    pred_weight_.resize(num_particle_, 1);

    cor_particle_.resize(4, num_particle_);
    cor_weight_.resize(num_particle_, 1);

    pred_weight_.setConstant(1.0/num_particle_);

    int particle_spread = std::sqrt(num_particle_);
    for (int i = 0; i < particle_spread; ++i)
        for (int j = 0; j < particle_spread; ++j)
            pred_particle_.col(i*particle_spread + j) << (surv_x_ / particle_spread) * i, 0, (surv_y_ / particle_spread) * j, 0;

    result_pred_particle_.resize(simulation_time_);
    result_pred_weight_.resize(simulation_time_);

    result_cor_particle_.resize(simulation_time_);
    result_cor_weight_.resize(simulation_time_);
}


void SIRParticleFilter::filteringStep()
{
    unsigned int k = getFilteringStep();


    if (k != 0)
        prediction_->predict(pred_particle_, pred_particle_);

    correction_->correct(pred_particle_, pred_weight_, measurement_.col(k),
                         cor_particle_, cor_weight_);

    cor_weight_ /= cor_weight_.sum();


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

        pred_particle_ = res_particle;
        pred_weight_   = res_weight;
    }
    else
    {
        pred_particle_ = cor_particle_;
        pred_weight_   = cor_weight_;
    }
}


void SIRParticleFilter::getResult()
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
