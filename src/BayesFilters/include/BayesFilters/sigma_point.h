#ifndef SIGMAPOINT_H
#define SIGMAPOINT_H

#include <BayesFilters/AdditiveStateModel.h>
#include <BayesFilters/Data.h>
#include <BayesFilters/ExogenousModel.h>
#include <BayesFilters/GaussianMixture.h>
#include <BayesFilters/LinearMeasurementModel.h>
#include <BayesFilters/MeasurementModel.h>
#include <BayesFilters/StateModel.h>

#include <functional>

#include <Eigen/Dense>


namespace bfl
{
namespace sigma_point
{
    using FunctionEvaluation = std::function<std::pair<bool, bfl::Data>(const Eigen::Ref<const Eigen::MatrixXf>&)>;
    
    struct UTWeight
    {
        Eigen::VectorXd mean;
        Eigen::VectorXd covariance;
        /**
         * c = sqrt(n + lambda) with lambda a ut parameter.
         */
        double c; 

        UTWeight(size_t n, const double alpha, const double beta, const double kappa);
    };

    void unscented_weights(const size_t n, const double alpha, const double beta, const double kappa, Eigen::Ref<Eigen::VectorXd> weight_mean, Eigen::Ref<Eigen::VectorXd> weight_covariance, double& c);

    Eigen::MatrixXd sigma_point(const GaussianMixture& state, const double c);

    std::tuple<bool, GaussianMixture, Eigen::MatrixXd> unscented_transform(const GaussianMixture& input, const UTWeight& weight, FunctionEvaluation function);

    std::pair<GaussianMixture, Eigen::MatrixXd> unscented_transform(const GaussianMixture& state, const UTWeight& weight, StateModel& state_model);

    std::pair<GaussianMixture, Eigen::MatrixXd> unscented_transform(const GaussianMixture& state, const UTWeight& weight, StateModel& state_model, ExogenousModel& exogenous_model);

    std::pair<GaussianMixture, Eigen::MatrixXd> unscented_transform(const GaussianMixture& state, const UTWeight& weight, AdditiveStateModel& state_model);

    std::pair<GaussianMixture, Eigen::MatrixXd> unscented_transform(const GaussianMixture& state, const UTWeight& weight, AdditiveStateModel& state_model, ExogenousModel& exogenous_model);

    std::pair<GaussianMixture, Eigen::MatrixXd> unscented_transform(const GaussianMixture& state, const UTWeight& weight, ExogenousModel& exogenous_model);

    std::tuple<bool, GaussianMixture, Eigen::MatrixXd> unscented_transform(const GaussianMixture& state, const UTWeight& weight, MeasurementModel& meas_model);

    std::tuple<bool, GaussianMixture, Eigen::MatrixXd> unscented_transform(const GaussianMixture& state, const UTWeight& weight, LinearMeasurementModel& meas_model);
}
}

#endif /* SIGMAPOINT_H */
