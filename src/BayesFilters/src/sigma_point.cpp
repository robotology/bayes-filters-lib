/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/sigma_point.h>
#include <BayesFilters/directional_statistics.h>

#include <Eigen/SVD>

using namespace bfl;
using namespace bfl::directional_statistics;
using namespace bfl::sigma_point;
using namespace Eigen;


bfl::sigma_point::UTWeight::UTWeight
(
    std::size_t n,
    const double alpha,
    const double beta,
    const double kappa
) :
    mean((2 * n) + 1),
    covariance((2 * n) + 1)
{
    unscented_weights(n, alpha, beta, kappa, mean, covariance, c);
}


void bfl::sigma_point::unscented_weights
(
    const std::size_t n,
    const double alpha,
    const double beta,
    const double kappa,
    Ref<VectorXd> weight_mean,
    Ref<VectorXd> weight_covariance,
    double& c
)
{
    double lambda = std::pow(alpha, 2.0) * (n + kappa) - n;

    for (int j = 0; j < ((2 * n) + 1); ++j)
    {
        if (j == 0)
        {
            weight_mean(j)       = lambda / (n + lambda);
            weight_covariance(j) = lambda / (n + lambda) + (1 - std::pow(alpha, 2.0) + beta);
        }
        else
        {
            weight_mean(j)       = 1 / (2 * (n + lambda));
            weight_covariance(j) = weight_mean(j);
        }
    }

    c = n + lambda;
}


MatrixXd bfl::sigma_point::sigma_point(const GaussianMixture& state, const double c)
{
    MatrixXd sigma_points(state.dim, ((state.dim * 2) + 1) * state.components);

    for (std::size_t i = 0; i < state.components; i++)
    {
        JacobiSVD<MatrixXd> svd = state.covariance(i).jacobiSvd(ComputeThinU);

        MatrixXd A = svd.matrixU() * svd.singularValues().cwiseSqrt().asDiagonal();

        Ref<MatrixXd> sp = sigma_points.middleCols(((state.dim * 2) + 1) * i, ((state.dim * 2) + 1));

        sp << VectorXd::Zero(state.dim), std::sqrt(c) * A, -std::sqrt(c) * A;

        if (state.dim_linear > 0)
            sp.topRows(state.dim_linear).colwise() += state.mean(i).topRows(state.dim_linear);

        if (state.dim_circular > 0)
            sp.middleRows(state.dim_linear, state.dim_circular) = directional_add(sp.middleRows(state.dim_linear, state.dim_circular), state.mean(i).middleRows(state.dim_linear, state.dim_circular));

        if (state.dim_noise > 0)
            sp.bottomRows(state.dim_noise).colwise() += state.mean(i).bottomRows(state.dim_noise);
    }

    return sigma_points;
}


std::tuple<bool, GaussianMixture, MatrixXd> bfl::sigma_point::unscented_transform
(
    const GaussianMixture& input,
    const UTWeight& weight,
    FunctionEvaluation function
)
{
    /* Sample sigma points. */
    MatrixXd input_sigma_points = sigma_point::sigma_point(input, weight.c);

    /* Propagate sigma points */
    bool valid_fun_data;
    Data fun_data;
    VectorDescription output_description;
    std::tie(valid_fun_data, fun_data, output_description) = function(input_sigma_points);

    /* Stop here if function evaluation failed. */
    if (!valid_fun_data)
        return std::make_tuple(false, GaussianMixture(), MatrixXd(0, 0));

    /* For now casting Data to MatrixXd. */
    MatrixXd prop_sigma_points = bfl::any::any_cast<MatrixXd&&>(std::move(fun_data));

    /* Initialize transformed gaussian. */
    GaussianMixture output(input.components, output_description.linear_components(), output_description.circular_components());

    /* Initialize cross covariance matrix. */
    MatrixXd cross_covariance(input.dim, output.dim * output.components);

    /* Process all the components of the mixture. */
    std::size_t base = ((input.dim * 2) + 1);
    for (std::size_t i = 0; i < input.components; i++)
    {
        Ref<MatrixXd> input_sigma_points_i = input_sigma_points.middleCols(base * i, base);
        Ref<MatrixXd> prop_sigma_points_i = prop_sigma_points.middleCols(base * i, base);

        /* Evaluate the mean. */
        output.mean(i).topRows(output.dim_linear).noalias() = prop_sigma_points_i.topRows(output.dim_linear) * weight.mean;
        output.mean(i).bottomRows(output.dim_circular) = directional_mean(prop_sigma_points_i.bottomRows(output.dim_circular), weight.mean);

        /* Evaluate the covariance. */
        prop_sigma_points_i.topRows(output.dim_linear).colwise() -= output.mean(i).topRows(output.dim_linear);
        prop_sigma_points_i.bottomRows(output.dim_circular) = directional_sub(prop_sigma_points_i.bottomRows(output.dim_circular), output.mean(i).bottomRows(output.dim_circular));
        output.covariance(i).noalias() = prop_sigma_points_i * weight.covariance.asDiagonal() * prop_sigma_points_i.transpose();

        /* Evaluate the input-output cross covariance matrix
           (noise components in the input are not considered). */
        Ref<MatrixXd> cross_covariance_i = cross_covariance.middleCols(output.dim * i, output.dim);
        input_sigma_points_i.topRows(input.dim_linear).colwise() -= input.mean(i).topRows(input.dim_linear);
        input_sigma_points_i.middleRows(input.dim_linear, input.dim_circular) = directional_sub(input_sigma_points_i.middleRows(input.dim_linear, input.dim_circular), input.mean(i).middleRows(input.dim_linear, input.dim_circular));
        cross_covariance_i.noalias() = input_sigma_points_i.topRows(input.dim_linear + input.dim_circular) * weight.covariance.asDiagonal() * prop_sigma_points_i.transpose();
    }

    return std::make_tuple(true, output, cross_covariance);
}


std::pair<GaussianMixture, MatrixXd> bfl::sigma_point::unscented_transform
(
    const GaussianMixture& state,
    const UTWeight& weight,
    StateModel& state_model
)
{
    FunctionEvaluation f = [&state_model](const Ref<const MatrixXd>& state)
                           {
                               MatrixXd tmp(state_model.getStateDescription().total_size(), state.cols());

                               state_model.motion(state, tmp);

                               return std::make_tuple(true, std::move(tmp), state_model.getStateDescription());
                           };
    MatrixXd cross_covariance;
    GaussianMixture output;
    std::tie(std::ignore, output, cross_covariance) = unscented_transform(state, weight, f);

    return std::make_pair(output, cross_covariance);
}


std::pair<GaussianMixture, MatrixXd> bfl::sigma_point::unscented_transform
(
    const GaussianMixture& state,
    const UTWeight& weight,
    AdditiveStateModel& state_model
)
{
    FunctionEvaluation f = [&state_model](const Ref<const MatrixXd>& state)
                           {
                               MatrixXd tmp(state.rows(), state.cols());

                               state_model.propagate(state, tmp);

                               return std::make_tuple(true, std::move(tmp), state_model.getStateDescription());
                           };

    MatrixXd cross_covariance;
    GaussianMixture output;
    std::tie(std::ignore, output, cross_covariance) = unscented_transform(state, weight, f);

    /* In the additive case the covariance matrix is augmented with the noise
       covariance matrix. */
    for(std::size_t i = 0; i < state.components; i++)
        output.covariance(i) += state_model.getNoiseCovarianceMatrix();

    return std::make_pair(output, cross_covariance);
}


std::tuple<bool, GaussianMixture, MatrixXd> bfl::sigma_point::unscented_transform
(
    const GaussianMixture& state,
    const UTWeight& weight,
    MeasurementModel& meas_model
)
{
    FunctionEvaluation f = [&meas_model](const Ref<const MatrixXd>& state)
                           {
                               bool valid_prediction;
                               bfl::Data prediction;

                               std::tie(valid_prediction, prediction) = meas_model.predictedMeasure(state);

                               return std::make_tuple(valid_prediction, std::move(prediction), meas_model.getMeasurementDescription());
                           };

    bool valid;
    MatrixXd cross_covariance;
    GaussianMixture output;
    std::tie(valid, output, cross_covariance) = unscented_transform(state, weight, f);

    return std::make_tuple(valid, output, cross_covariance);
}


std::tuple<bool, GaussianMixture, MatrixXd> bfl::sigma_point::unscented_transform
(
    const GaussianMixture& state,
    const UTWeight& weight,
    AdditiveMeasurementModel& meas_model
)
{
    FunctionEvaluation f = [&meas_model](const Ref<const MatrixXd>& state)
                           {
                               bool valid_prediction;
                               bfl::Data prediction;

                               std::tie(valid_prediction, prediction) = meas_model.predictedMeasure(state);

                               return std::make_tuple(valid_prediction, std::move(prediction), meas_model.getMeasurementDescription());
                           };

    bool valid;
    MatrixXd cross_covariance;
    GaussianMixture output;
    std::tie(valid, output, cross_covariance) = unscented_transform(state, weight, f);

    /* In the additive case the covariance matrix is augmented with the noise
       covariance matrix. */
    MatrixXd noise_cov;
    std::tie(std::ignore, noise_cov) = meas_model.getNoiseCovarianceMatrix();
    for (std::size_t i = 0; i < state.components; i++)
        output.covariance(i) += noise_cov;

    return std::make_tuple(valid, output, cross_covariance);
}
