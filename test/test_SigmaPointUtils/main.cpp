#include <iostream>
#include <thread>

#include <BayesFilters/sigmapointutils.h>

using namespace bfl;
using namespace Eigen;


int main()
{
    std::cout << "Running Unscented Weights..." << std::endl;

    unsigned int n     = 4;
    double       alpha = 1.0;
    double       beta  = 2.0;
    double       kappa = 0;

    VectorXd wight_mean((2 * n) + 1);
    VectorXd wight_covariance((2 * n) + 1);
    double   c;

    UnscentedWeights(n, alpha, beta, kappa,
                     wight_mean, wight_covariance, c);

    std::cout << "Weight mean:\n" << wight_mean << std::endl;
    std::cout << "Weight sum: " << wight_mean.sum() << std::endl << std::endl;

    std::cout << "Covariance mean:\n" << wight_covariance << std::endl;
    std::cout << "Covariance sum: " << wight_covariance.sum() << std::endl << std::endl;

    std::cout << "c: " << c << std::endl << std::endl;

    VectorXd wight_mean_test((2 * n) + 1);
    wight_mean_test << 0, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125;

    if ((wight_mean - wight_mean_test).cwiseAbs().sum() > 0.0001)
    {
        std::cerr << "Wrong unscented weights or the mean." << std::endl;
        return EXIT_FAILURE;
    }

    VectorXd wight_covariance_test((2 * n) + 1);
    wight_covariance_test << 2, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125;

    if ((wight_covariance_test - wight_covariance_test).cwiseAbs().sum() > 0.0001)
    {
        std::cerr << "Wrong unscented weights or the mean." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "done!\n" << std::endl;

    return EXIT_SUCCESS;
}
