#include <fstream>
#include <iostream>
#include <utility>

#include <Eigen/Dense>

#include "BayesFilters/ParticleFilterCorrection.h"
#include "BayesFilters/SIRParticleFilter.h"

using namespace bfl;
using namespace Eigen;

    
SIRParticleFilter::SIRParticleFilter(std::unique_ptr<ParticleFilterPrediction> prediction, std::unique_ptr<Correction> correction, std::unique_ptr<Resampling> resampling) noexcept :
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
    correction_->observeState(object_.col(0), measurement_.col(0));
    for (int k = 1; k < simulation_time_; ++k)
    {
        prediction_->predict(object_.col(k-1), object_.col(k));
        correction_->measureState(object_.col(k), measurement_.col(k));
    }

    /* INITIALIZE FILTER */
    init_weight_.resize(num_particle_, 1);
    init_weight_.setConstant(1.0/num_particle_);

    int particle_spread = std::sqrt(num_particle_);
    init_particle_.resize(4, num_particle_);
    for (int i = 0; i < particle_spread; ++i)
    {
        for (int j = 0; j < particle_spread; ++j)
        {
            init_particle_.col(i*particle_spread + j) << (surv_x_ / particle_spread) * i,
            0,
            (surv_y_ / particle_spread) * j,
            0;
        }
    }

    result_particle_.resize(simulation_time_);
    result_weight_.resize(simulation_time_);
}


void SIRParticleFilter::filteringStep()
{
    unsigned int k = getFilteringStep();

    for (int i = 0; i < num_particle_; ++i)
        prediction_->predict(init_particle_.col(i), init_particle_.col(i));

    for (int i = 0; i < num_particle_; ++i)
        correction_->correct(init_particle_.col(i), measurement_.col(k), init_weight_.row(i));

    init_weight_ /= init_weight_.sum();

    result_particle_[k] = init_particle_;
    result_weight_  [k] = init_weight_;

    if (resampling_->neff(init_weight_) < num_particle_/3)
    {
        MatrixXf temp_particle(4, num_particle_);
        VectorXf temp_weight(num_particle_, 1);
        VectorXf temp_parent(num_particle_, 1);

        resampling_->resample(init_particle_, init_weight_, temp_particle, temp_weight, temp_parent);

        init_particle_ = temp_particle;
        init_weight_   = temp_weight;
    }
}


void SIRParticleFilter::getResult()
{
    std::ofstream result_file_object;
    std::ofstream result_file_measurement;
    std::ofstream result_file_particle;
    std::ofstream result_file_weight;

    result_file_object.open     ("./result_object.txt");
    result_file_measurement.open("./result_measurement.txt");
    result_file_particle.open   ("./result_particle.txt");
    result_file_weight.open     ("./result_weight.txt");

    result_file_object       << object_;
    result_file_measurement  << measurement_;
    for (unsigned int k = 0; k < getFilteringStep(); ++k)
    {
        result_file_particle << result_particle_[k] << std::endl << std::endl;
        result_file_weight   << result_weight_[k] << std::endl << std::endl;
    }

    result_file_object.close();
    result_file_measurement.close();
    result_file_particle.close();
    result_file_weight.close();
}
