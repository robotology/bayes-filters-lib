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


void SIRParticleFilter::runFilter()
{
    /* INITIALIZATION */
    int simulation_time = 100;
    int num_particle    = 900;
    int surv_x          = 1000;
    int surv_y          = 1000;

    /* GENERATE MEASUREMENTS */
    measurement_.resize(2, simulation_time);
    object_.resize(4, simulation_time);

    object_.col(0) << 0, 10, 0, 10;
    correction_->virtual_observation(object_.col(0), measurement_.col(0));
    for (int k = 1; k < simulation_time; ++k)
    {
        prediction_->predict(object_.col(k-1), object_.col(k));
        dynamic_cast<ParticleFilterCorrection*>(correction_.get())->observation(object_.col(k), measurement_.col(k));
    }

    /* INITIALIZE FILTER */
    init_weight_.resize(num_particle, 1);
    init_weight_.setConstant(1.0/num_particle);

    int particle_spread = std::sqrt(num_particle);
    init_particle_.resize(4, num_particle);
    for (int i = 0; i < particle_spread; ++i)
    {
        for (int j = 0; j < particle_spread; ++j)
        {
            init_particle_.col(i*particle_spread + j) << (surv_x / particle_spread) * i,
                                                                                      0,
                                                         (surv_y / particle_spread) * j,
                                                                                      0;
        }
    }

    result_particle_.resize(simulation_time);
    result_weight_.resize(simulation_time);

    /* FILTERING */
    for (int k = 0; k < simulation_time; ++k)
    {
        //Snapshot();

        for (int i = 0; i < num_particle; ++i)
            prediction_->predict(init_particle_.col(i), init_particle_.col(i));

        //Snapshot();

        for (int i = 0; i < num_particle; ++i)
            correction_->correct(init_particle_.col(i), measurement_.col(k), init_weight_.row(i));

        init_weight_ /= init_weight_.sum();

        //Snapshot();

        result_particle_[k] = init_particle_;
        result_weight_  [k] = init_weight_;

        if (resampling_->neff(init_weight_) < num_particle/3)
        {
            MatrixXf temp_particle(4, num_particle);
            VectorXf temp_weight(num_particle, 1);
            VectorXf temp_parent(num_particle, 1);

            resampling_->resample(init_particle_, init_weight_, temp_particle, temp_weight, temp_parent);

            init_particle_ = temp_particle;
            init_weight_   = temp_weight;
        }

    }
}


void SIRParticleFilter::snapshot()
{
    std::ofstream result_file_object;
    std::ofstream result_file_measurement;
    std::ofstream result_file_particle;
    std::ofstream result_file_weight;

    result_file_object.open     ("../../../../dbg/result_object.txt"     , std::ios_base::out | std::ios_base::trunc);
    result_file_measurement.open("../../../../dbg/result_measurement.txt", std::ios_base::out | std::ios_base::trunc);
    result_file_particle.open   ("../../../../dbg/result_particle.txt"   , std::ios_base::out | std::ios_base::trunc);
    result_file_weight.open     ("../../../../dbg/result_weight.txt"     , std::ios_base::out | std::ios_base::trunc);

    result_file_object      << object_;
    result_file_measurement << measurement_;
    result_file_particle    << init_particle_;
    result_file_weight      << init_weight_;

    result_file_object.close();
    result_file_measurement.close();
    result_file_particle.close();
    result_file_weight.close();
}


void SIRParticleFilter::getResult()
{
    std::ofstream result_file_object;
    std::ofstream result_file_measurement;
    std::ofstream result_file_particle;
    std::ofstream result_file_weight;

    result_file_object.open     ("../../../../dbg/result_object.txt");
    result_file_measurement.open("../../../../dbg/result_measurement.txt");
    result_file_particle.open   ("../../../../dbg/result_particle.txt");
    result_file_weight.open     ("../../../../dbg/result_weight.txt");

    result_file_object       << object_;
    result_file_measurement  << measurement_;
    for (int k = 0; k < 100; ++k)
    {
        result_file_particle << result_particle_[k] << std::endl << std::endl;
        result_file_weight   << result_weight_[k] << std::endl << std::endl;
    }

    result_file_object.close();
    result_file_measurement.close();
    result_file_particle.close();
    result_file_weight.close();
}
