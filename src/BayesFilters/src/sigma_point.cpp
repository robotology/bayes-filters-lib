/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/sigma_point.h>
#include <BayesFilters/directional_statistics.h>
#include <BayesFilters/utils.h>

#include <Eigen/SVD>

using namespace bfl;
using namespace bfl::directional_statistics;
using namespace bfl::sigma_point;
using namespace bfl::utils;
using namespace Eigen;


bfl::sigma_point::UTWeight::UTWeight
(
    std::size_t dof,
    const double alpha,
    const double beta,
    const double kappa
) :
    mean((2 * dof) + 1),
    covariance((2 * dof) + 1)
{
    unscented_weights(dof, alpha, beta, kappa, mean, covariance, c);
}


bfl::sigma_point::UTWeight::UTWeight
(
    const VectorDescription& vector_description,
    const double alpha,
    const double beta,
    const double kappa
)
{
    /* Degree of freedom associated to input space. */
    std::size_t dof = vector_description.dof_size();

    mean.resize((2 * dof) + 1);
    covariance.resize((2 * dof) + 1);

    unscented_weights(dof, alpha, beta, kappa, mean, covariance, c);
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
    MatrixXd sigma_points(state.dim, ((state.dim_covariance * 2) + 1) * state.components);

    for (std::size_t i = 0; i < state.components; i++)
    {
        JacobiSVD<MatrixXd> svd = state.covariance(i).jacobiSvd(ComputeThinU);

        MatrixXd A = svd.matrixU() * svd.singularValues().cwiseSqrt().asDiagonal();

        Ref<MatrixXd> sp = sigma_points.middleCols(((state.dim_covariance * 2) + 1) * i, ((state.dim_covariance * 2) + 1));

        MatrixXd perturbations(state.dim_covariance, (state.dim_covariance * 2) + 1);
        perturbations << VectorXd::Zero(state.dim_covariance), std::sqrt(c) * A, -std::sqrt(c) * A;

        if (state.dim_linear > 0)
            sp.topRows(state.dim_linear) = perturbations.topRows(state.dim_linear).colwise() + state.mean(i).topRows(state.dim_linear);

        if (state.dim_circular > 0)
        {
            if (state.use_quaternion)
                for (std::size_t j = 0; j < state.dim_circular; j++)
                {
                    /* Enforce first sigma point to be the mean quaternion. */
                    sp.middleRows(state.dim_linear + j * 4, 4).col(0) = state.mean(i).middleRows(state.dim_linear + j * 4, 4);

                    /* Evaluate the remaining sigma points as perturbation of the mean quaternion. */
                    sp.middleRows(state.dim_linear + j * 4, 4).rightCols(2 * state.dim_covariance) = sum_quaternion_rotation_vector(state.mean(i).middleRows(state.dim_linear + j * 4, 4), perturbations.middleRows(state.dim_linear + j * 3, 3).rightCols(2 * state.dim_covariance));
                }
            else
                sp.middleRows(state.dim_linear, state.dim_circular) = directional_add(perturbations.middleRows(state.dim_linear, state.dim_circular), state.mean(i).middleRows(state.dim_linear, state.dim_circular));
        }

        if (state.dim_noise > 0)
            sp.bottomRows(state.dim_noise) = perturbations.bottomRows(state.dim_noise).colwise() + state.mean(i).bottomRows(state.dim_noise);
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

    /* Unscented transforms are available only for vector-valued functions. Hence, casting Data to MatrixXd. */
    MatrixXd prop_sigma_points = bfl::any::any_cast<MatrixXd&&>(std::move(fun_data));

    /* Initialize transformed gaussian. */
    GaussianMixture output(input.components, output_description.linear_components(), output_description.circular_components(), output_description.circular_type == VectorDescription::CircularType::Quaternion);

    /* Initialize cross covariance matrix (noise components in the input are not considered). */
    MatrixXd cross_covariance(input.dim_covariance - input.dim_noise, output.dim_covariance * output.components);

    /* Process all the components of the mixture. */
    std::size_t base = ((input.dim_covariance * 2) + 1);
    for (std::size_t i = 0; i < input.components; i++)
    {
        const Ref<MatrixXd> input_sigma_points_i = input_sigma_points.middleCols(base * i, base);
        const Ref<MatrixXd> prop_sigma_points_i = prop_sigma_points.middleCols(base * i, base);

        /* Evaluate the mean. */
        output.mean(i).topRows(output.dim_linear).noalias() = prop_sigma_points_i.topRows(output.dim_linear) * weight.mean;

        if (output.dim_circular > 0)
        {
            if (output.use_quaternion)
                for (std::size_t j = 0; j < output.dim_circular; j++)
                    output.mean(i).middleRows(output.dim_linear + j * 4, 4) = mean_quaternion(weight.mean, prop_sigma_points_i.middleRows(output.dim_linear + j * 4, 4));
            else
                output.mean(i).bottomRows(output.dim_circular) = directional_mean(prop_sigma_points_i.bottomRows(output.dim_circular), weight.mean);
        }

        /* Evaluate the covariance. */
        MatrixXd offsets_from_mean(output.dim_covariance, input_sigma_points_i.cols());

        offsets_from_mean.topRows(output.dim_linear) = prop_sigma_points_i.topRows(output.dim_linear).colwise() - output.mean(i).topRows(output.dim_linear);
        if (output.dim_circular > 0)
        {
            if (output.use_quaternion)
                for (std::size_t j = 0; j < output.dim_circular; j++)
                    offsets_from_mean.middleRows(output.dim_linear + j * 3, 3) = diff_quaternion(prop_sigma_points_i.middleRows(output.dim_linear + j * 4, 4), output.mean(i).middleRows(output.dim_linear + j * 4, 4));
            else
                offsets_from_mean.bottomRows(output.dim_circular) = directional_sub(prop_sigma_points_i.bottomRows(output.dim_circular), output.mean(i).bottomRows(output.dim_circular));
        }
        output.covariance(i).noalias() = offsets_from_mean * weight.covariance.asDiagonal() * offsets_from_mean.transpose();

        /* Evaluate the input-output cross covariance matrix (noise components in the input are not considered). */
        Ref<MatrixXd> cross_covariance_i = cross_covariance.middleCols(output.dim_covariance * i, output.dim_covariance);
        MatrixXd input_offsets_from_mean(input.dim_covariance - input.dim_noise, input_sigma_points_i.cols());
        input_offsets_from_mean.topRows(input.dim_linear) = input_sigma_points_i.topRows(input.dim_linear).colwise() - input.mean(i).topRows(input.dim_linear);
        if (input.dim_circular > 0)
        {
            if (input.use_quaternion)
                for (std::size_t j = 0; j < input.dim_circular; j++)
                    input_offsets_from_mean.middleRows(input.dim_linear + j * 3, 3) = diff_quaternion(input_sigma_points_i.middleRows(input.dim_linear + j * 4, 4), input.mean(i).middleRows(input.dim_linear + j * 4, 4));
            else
                input_offsets_from_mean.bottomRows(input.dim_circular) = directional_sub(input_sigma_points_i.middleRows(input.dim_linear, input.dim_circular), input.mean(i).middleRows(input.dim_linear, input.dim_circular));
        }
        cross_covariance_i.noalias() = input_offsets_from_mean * weight.covariance.asDiagonal() * offsets_from_mean.transpose();
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
