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
            sp.bottomRows(state.dim_circular) = directional_add(sigma_points.bottomRows(state.dim_circular), state.mean(i).bottomRows(state.dim_circular));
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
    Data fun_data;
    bool valid_fun_data;
    std::tie(valid_fun_data, fun_data) = function(input_sigma_points.cast<float>());

    /* Stop here if function evaluation failed. */
    if (!valid_fun_data)
        return std::make_tuple(false, GaussianMixture(), MatrixXd(0, 0));

    /* For now casting Data to MatrixXd. */
    MatrixXd prop_sigma_points = (bfl::any::any_cast<MatrixXf&&>(std::move(fun_data))).cast<double>();

    /* Initialize transformed gaussian. */
    GaussianMixture output(input.components, prop_sigma_points.rows());

    /* Initialize cross covariance matrix. */
    MatrixXd cross_covariance(input.dim, output.dim * output.components);

    /* Process all the components of the mixture. */
    std::size_t base = ((input.dim * 2) + 1);
    for (std::size_t i = 0; i < input.components; i++)
    {
        Ref<MatrixXd> input_sigma_points_i = input_sigma_points.middleCols(base * i, base);
        Ref<MatrixXd> prop_sigma_points_i = prop_sigma_points.middleCols(base * i, base);

        /* Evaluate the mean. */
        output.mean(i).noalias() = prop_sigma_points_i * weight.mean;

        /* Evaluate the covariance. */
        prop_sigma_points_i.colwise() -= output.mean(i);
        output.covariance(i).noalias() = prop_sigma_points_i * weight.covariance.asDiagonal() * prop_sigma_points_i.transpose();

        /* Evaluate the input-output cross covariance matrix. */
        Ref<MatrixXd> cross_covariance_i = cross_covariance.middleCols(output.dim * i, output.dim);
        input_sigma_points_i.colwise() -= input.mean(i);
        cross_covariance_i.noalias() = input_sigma_points_i * weight.covariance.asDiagonal() * prop_sigma_points_i.transpose();
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
    FunctionEvaluation f = [&state_model](const Ref<const MatrixXf>& state)
                           {
                               MatrixXf tmp(state.rows(), state.cols());
                               
                               state_model.motion(state, tmp);
                               
                               return std::make_pair(true, std::move(tmp));
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
    StateModel& state_model,
    ExogenousModel& exogenous_model
)
{
    FunctionEvaluation f = [&state_model, &exogenous_model](const Ref<const MatrixXf>& state)
                           {
                               MatrixXf tmp_state(state.rows(), state.cols());
                               state_model.motion(state, tmp_state);

                               MatrixXf tmp_exog(tmp_state.rows(), tmp_state.cols());
                               exogenous_model.propagate(tmp_state, tmp_exog);
                               
                               return std::make_pair(true, std::move(tmp_exog));
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
    FunctionEvaluation f = [&state_model](const Ref<const MatrixXf>& state)
                           {
                               MatrixXf tmp(state.rows(), state.cols());
                               
                               state_model.propagate(state, tmp);
                               
                               return std::make_pair(true, std::move(tmp));
                           };

    MatrixXd cross_covariance;
    GaussianMixture output;
    std::tie(std::ignore, output, cross_covariance) = unscented_transform(state, weight, f);

    /* In the additive case the covariance matrix is augmented with the noise
       covariance matrix. */
    for(std::size_t i = 0; i < state.components; i++)
        output.covariance(i) += state_model.getNoiseCovarianceMatrix().cast<double>();

    return std::make_pair(output, cross_covariance);
}


std::pair<GaussianMixture, MatrixXd> bfl::sigma_point::unscented_transform
(
    const GaussianMixture& state,
    const UTWeight& weight,
    AdditiveStateModel& state_model,
    ExogenousModel& exogenous_model
)
{
    FunctionEvaluation f = [&state_model, &exogenous_model](const Ref<const MatrixXf>& state)
                           {
                               MatrixXf tmp_state(state.rows(), state.cols());
                               state_model.propagate(state, tmp_state);

                               MatrixXf tmp_exog(tmp_state.rows(), tmp_state.cols());
                               exogenous_model.propagate(tmp_state, tmp_exog);

                               return std::make_pair(true, std::move(tmp_exog));
                           };

    bool valid;
    MatrixXd cross_covariance;
    GaussianMixture output;
    std::tie(valid, output, cross_covariance) = unscented_transform(state, weight, f);

    /* In the additive case the covariance matrix is augmented with the noise
       covariance matrix. */
    for (std::size_t i = 0; i < state.components; i++)
        output.covariance(i) += state_model.getNoiseCovarianceMatrix().cast<double>();

    return std::make_pair(output, cross_covariance);
}


std::pair<GaussianMixture, MatrixXd> bfl::sigma_point::unscented_transform
(
    const GaussianMixture& state,
    const UTWeight& weight,
    ExogenousModel& exogenous_model
)
{
    FunctionEvaluation f = [&exogenous_model](const Ref<const MatrixXf>& state)
                           {
                               MatrixXf tmp(state.rows(), state.cols());
                               
                               exogenous_model.propagate(state, tmp);
                               
                               return std::make_pair(true, std::move(tmp));
                           };

    bool valid;
    MatrixXd cross_covariance;
    GaussianMixture output;
    std::tie(valid, output, cross_covariance) = unscented_transform(state, weight, f);

    return std::make_pair(output, cross_covariance);
}


std::tuple<bool, GaussianMixture, MatrixXd> bfl::sigma_point::unscented_transform
(
    const GaussianMixture& state,
    const UTWeight& weight,
    MeasurementModel& meas_model
)
{
    FunctionEvaluation f = [&meas_model](const Ref<const MatrixXf>& state)
                           {
                               return meas_model.predictedMeasure(state);
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
    LinearMeasurementModel& meas_model
)
{
    FunctionEvaluation f = [&meas_model](const Ref<const MatrixXf>& state)
                           {
                               return meas_model.predictedMeasure(state);
                           };

    bool valid;
    MatrixXd cross_covariance;
    GaussianMixture output;
    std::tie(valid, output, cross_covariance) = unscented_transform(state, weight, f);

    /* In the additive case the covariance matrix is augmented with the noise
       covariance matrix. */
    MatrixXf noise_cov;
    std::tie(std::ignore, noise_cov) = meas_model.getNoiseCovarianceMatrix();
    for (std::size_t i = 0; i < state.components; i++)
        output.covariance(i) += noise_cov.cast<double>();

    return std::make_tuple(valid, output, cross_covariance);
}
