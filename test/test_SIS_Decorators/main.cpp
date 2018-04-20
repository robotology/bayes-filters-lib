#include <iostream>
#include <memory>

#include <BayesFilters/DrawParticles.h>
#include <BayesFilters/LinearSensor.h>
#include <BayesFilters/ObservationModelDecorator.h>
#include <BayesFilters/PFCorrectionDecorator.h>
#include <BayesFilters/PFPredictionDecorator.h>
#include <BayesFilters/Resampling.h>
#include <BayesFilters/StateModelDecorator.h>
#include <BayesFilters/SIS.h>
#include <BayesFilters/UpdateParticles.h>
#include <BayesFilters/WhiteNoiseAcceleration.h>

using namespace bfl;


class DecoratedWNA : public StateModelDecorator
{
public:
    DecoratedWNA(std::unique_ptr<StateModel> state_model) noexcept :
        StateModelDecorator(std::move(state_model)) { };


    void motion(const Eigen::Ref<const Eigen::MatrixXf>& cur_states, Eigen::Ref<Eigen::MatrixXf> mot_states) override
    {
        std::cout << "Decorator: DecoratedWNA::motion()." << std::endl;

        StateModelDecorator::motion(cur_states, mot_states);
    }
};


class DecoratedLinearSensor : public MeasurementModelDecorator
{
public:
    DecoratedLinearSensor(std::unique_ptr<MeasurementModel> observation_model) noexcept :
        MeasurementModelDecorator(std::move(observation_model)) { }


    void measure(const Eigen::Ref<const Eigen::MatrixXf>& cur_states, Eigen::Ref<Eigen::MatrixXf> measurements) override
    {
        std::cout << "Decorator: DecoratedLinearSensor::measure()." << std::endl;

        MeasurementModelDecorator::measure(cur_states, measurements);
    }
};


class DecoratedDrawParticles : public PFPredictionDecorator
{
public:
    DecoratedDrawParticles(std::unique_ptr<PFPrediction> prediction) noexcept :
        PFPredictionDecorator(std::move(prediction)) { }

protected:
    void predictStep(const Eigen::Ref<const Eigen::MatrixXf>& prev_states, const Eigen::Ref<const Eigen::VectorXf>& prev_weights,
                     Eigen::Ref<Eigen::MatrixXf> pred_states, Eigen::Ref<Eigen::VectorXf> pred_weights) override
    {
        std::cout << "Decorator: DecoratedDrawParticles::predictStep()." << std::endl;

        PFPredictionDecorator::predictStep(prev_states, prev_weights, pred_states, pred_weights);
    }
};


class DecoratedUpdateParticles : public PFCorrectionDecorator
{
public:
    DecoratedUpdateParticles(std::unique_ptr<PFCorrection> correction) noexcept :
        PFCorrectionDecorator(std::move(correction)) { }

protected:
    void correctStep(const Eigen::Ref<const Eigen::MatrixXf>& pred_states, const Eigen::Ref<const Eigen::VectorXf>& pred_weights, const Eigen::Ref<const Eigen::MatrixXf>& measurements,
                     Eigen::Ref<Eigen::MatrixXf> cor_states, Eigen::Ref<Eigen::VectorXf> cor_weights) override
    {
        std::cout << "Decorator: DecoratedUpdateParticles::correctStep()." << std::endl;

        PFCorrectionDecorator::correctStep(pred_states, pred_weights, measurements, cor_states, cor_weights);
    }
};


int main()
{
    /* Initialize a white noise acceleration motion model */
    std::unique_ptr<WhiteNoiseAcceleration> wna(new WhiteNoiseAcceleration());

    /* Initialize a white noise acceleration decorator */
    std::unique_ptr<DecoratedWNA> decorated_wna(new DecoratedWNA(std::move(wna)));


    /* Pass ownership of the motion model to the prediction step */
    std::unique_ptr<DrawParticles> pf_prediction(new DrawParticles());
    pf_prediction->setStateModel(std::move(decorated_wna));

    /* Initialize a draw particle decorator */
    std::unique_ptr<DecoratedDrawParticles> decorated_prediction(new DecoratedDrawParticles(std::move(pf_prediction)));


    /* Initialize a linear sensor (provides direct observation of the state) */
    std::unique_ptr<LinearSensor> lin_sense(new LinearSensor());

    /* Initialize a white noise acceleration decorator */
    std::unique_ptr<DecoratedLinearSensor> decorated_linearsensor(new DecoratedLinearSensor(std::move(lin_sense)));


    /* Pass ownership of the observation model (the sensor) to the prediction step */
    std::unique_ptr<UpdateParticles> pf_correction(new UpdateParticles());
    pf_correction->setObservationModel(std::move(decorated_linearsensor));

    /* Initialize a update particle decorator */
    std::unique_ptr<DecoratedUpdateParticles> decorated_correction(new DecoratedUpdateParticles(std::move(pf_correction)));


    /* Initialize a resampling algorithm */
    std::unique_ptr<Resampling> resampling(new Resampling());


    std::cout << "Constructing SIS particle filter..." << std::flush;
    SIS sis_pf;
    sis_pf.setPrediction(std::move(decorated_prediction));
    sis_pf.setCorrection(std::move(decorated_correction));
    sis_pf.setResampling(std::move(resampling));
    std::cout << "done!" << std::endl;


    std::cout << "Preparing SIS particle filter..." << std::flush;
    sis_pf.boot();
    std::cout << "completed!" << std::endl;


    std::cout << "Running SIS particle filter..." << std::flush;
    sis_pf.run();
    std::cout << "waiting..." << std::flush;
    if (!sis_pf.wait())
        return EXIT_FAILURE;
    std::cout << "completed!" << std::endl;


    std::cout << "Storing filtering results..." << std::flush;
    sis_pf.getResult();
    std::cout << "done!" << std::endl;


    return EXIT_SUCCESS;
}
