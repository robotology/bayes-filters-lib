#include <cmath>
#include <iostream>
#include <thread>

#include <BayesFilters/Gaussian.h>
#include <BayesFilters/sigma_point.h>

using namespace bfl;
using namespace bfl::sigma_point;
using namespace Eigen;


int main()
{
    std::cout << "Running Unscented Weights..." << std::endl;

    unsigned int n     = 4;
    double       alpha = 1.0;
    double       beta  = 2.0;
    double       kappa = 0;

    UTWeight weight(n, alpha, beta, kappa);

    std::cout << "Weight mean:\n" << weight.mean << std::endl;
    std::cout << "Weight sum: " << weight.mean.sum() << std::endl << std::endl;

    std::cout << "Covariance mean:\n" << weight.covariance << std::endl;
    std::cout << "Covariance sum: " << weight.covariance.sum() << std::endl << std::endl;

    std::cout << "c: " << weight.c << std::endl << std::endl;

    VectorXd weight_mean_test((2 * n) + 1);
    weight_mean_test << 0, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125;

    if ((weight.mean - weight_mean_test).cwiseAbs().sum() > 0.0001)
    {
        std::cerr << "Wrong unscented weights of the mean." << std::endl;
        return EXIT_FAILURE;
    }
    else
        std::cout << "Correct unscented weights of the mean." << std::endl;

    VectorXd weight_covariance_test((2 * n) + 1);
    weight_covariance_test << 2, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125;

    if ((weight.covariance - weight_covariance_test).cwiseAbs().sum() > 0.0001)
    {
        std::cerr << "Wrong unscented weights of the covariance." << std::endl;
        return EXIT_FAILURE;
    }
        std::cout << "Correct unscented weights of the covariance." << std::endl;


    Gaussian gaussian(3, 3);
    gaussian.mean() << 1.0, 1.0, 1.0, M_PI, M_PI_2, M_PI_4;
    gaussian.covariance() << 1.0, 0,   0,   0,     0,     0,
                             0,   2.0, 0,   0,     0,     0,
                             0,   0,   4.0, 0,     0,     0,
                             0,   0,   0,   0.157, 0,     0,
                             0,   0,   0,   0,     0.157, 0,
                             0,   0,   0,   0,     0,     0.157;

    MatrixXd sigma_points = sigma_point::sigma_point(gaussian, weight.c);

    std::cout << "Gaussian mean:\n" << gaussian.mean() << "\nGaussian covariance:\n" << gaussian.covariance() << "\nc: " << weight.c << std::endl;

    std::cout << "Sigma points:\n" << sigma_points << std::endl;

    VectorXd sigma_point_test_r0((2 * gaussian.dim) + 1);
    VectorXd sigma_point_test_r3((2 * gaussian.dim) + 1);
    sigma_point_test_r0 << 1, 1, 1, 3, 1, 1, 1, 1, 1, -1, 1, 1, 1;
    sigma_point_test_r3 << 3.14159, 3.14159, 3.14159, 3.14159, -2.34913, 3.14159, 3.14159, 3.14159, 3.14159, 3.14159, 2.34913, 3.14159, 3.14159;

    if (((sigma_points.row(0) - sigma_point_test_r0.transpose()).cwiseAbs().sum() > 0.0001) ||
        ((sigma_points.row(3) - sigma_point_test_r3.transpose()).cwiseAbs().sum() > 0.0001)   )
    {
        std::cerr << "Wrong sigma points." << std::endl;
        return EXIT_FAILURE;
    }
        std::cout << "Correct sigma points." << std::endl;


    std::cout << "done!\n" << std::endl;

    return EXIT_SUCCESS;
}
