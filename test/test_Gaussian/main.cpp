#include <chrono>
#include <iostream>
#include <thread>

#include <BayesFilters/Gaussian.h>
#include <BayesFilters/GaussianMixture.h>

using namespace bfl;


int main()
{
    std::cout << "Constructing an empty Gaussian..." << std::endl;

    bool catch_exception = false;
    try
    {
        Gaussian(0);
    }
    catch (const std::runtime_error& e)
    {
        catch_exception = true;
        std::cout << "Caught error.\n" << e.what() << std::endl;
    }

    if (!catch_exception)
        return EXIT_FAILURE;

    std::cout << "...done!\n" << std::endl;



    std::cout << "Constructing a 1D Gaussian..." << std::endl;

    Gaussian gaussian_1d;

    if (!(gaussian_1d.mean.array() == 0).all())
    {
        std::cerr << "Default initialization of Gaussian mean is not zero, is\n" << gaussian_1d.mean << std::endl;
        return EXIT_FAILURE;
    }
    else
        std::cout << "Default initialization of Gaussian mean is all zero:\n" << gaussian_1d.mean << std::endl;

    if (!(gaussian_1d.covariance.array() == 0).all())
    {
        std::cerr << "Default initialization of Gaussian covariance is not zero, is\n" << gaussian_1d.covariance << std::endl;
        return EXIT_FAILURE;
    }
    else
        std::cout << "Default initialization of Gaussian covariance is all zero:\n" << gaussian_1d.covariance << std::endl;

    if (!(gaussian_1d.weight == 1.0))
    {
        std::cerr << "Default initialization of Gaussian waight is not 1.0, is\n" << gaussian_1d.weight << std::endl;
        return EXIT_FAILURE;
    }
    else
        std::cout << "Default initialization of Gaussian covariance is 1.0: " << gaussian_1d.weight << std::endl;

    std::cout << "done!\n" << std::endl;



    std::cout << "Constructing a 3D Gaussian..." << std::endl;

    Gaussian gaussian_3d(3);

    if (!(gaussian_3d.mean.array() == 0).all())
    {
        std::cerr << "Default initialization of Gaussian mean is not zero, is\n" << gaussian_3d.mean << std::endl;
        return EXIT_FAILURE;
    }
    else
        std::cout << "Default initialization of Gaussian mean is all zero:\n" << gaussian_3d.mean << std::endl;

    if (!(gaussian_3d.covariance.array() == 0).all())
    {
        std::cerr << "Default initialization of Gaussian covariance is not zero, is\n" << gaussian_3d.covariance << std::endl;
        return EXIT_FAILURE;
    }
    else
        std::cout << "Default initialization of Gaussian covariance is all zero:\n" << gaussian_3d.covariance << std::endl;

    if (!(gaussian_3d.weight == 1.0))
    {
        std::cerr << "Default initialization of Gaussian waight is not 1.0, is\n" << gaussian_3d.weight << std::endl;
        return EXIT_FAILURE;
    }
    else
        std::cout << "Default initialization of Gaussian covariance is 1.0: " << gaussian_3d.weight << std::endl;

    std::cout << "done!\n" << std::endl;



    std::cout << "Constructing a Gaussian mixture with 5 3D components..." << std::endl;

    GaussianMixture gaussian_mixture(5, 3);

    for (int i = 0; i < gaussian_mixture.components; ++i)
    {
        if (!(gaussian_mixture[i].mean.array() == 0).all())
        {
            std::cerr << "Default initialization of Gaussian mean (" << i + 1 << ") is not zero, is\n" << gaussian_mixture[i].mean << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Default initialization of Gaussian mean (" << i + 1 << ") is all zero:\n" << gaussian_mixture[i].mean << std::endl;

        if (!(gaussian_mixture[i].covariance.array() == 0).all())
        {
            std::cerr << "Default initialization of Gaussian covariance (" << i + 1 << ") is not zero, is\n" << gaussian_mixture[i].covariance << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Default initialization of Gaussian covariance (" << i + 1 << ") is all zero:\n" << gaussian_mixture[i].covariance << std::endl;

        if (!(gaussian_mixture[i].weight ==  1.0 / gaussian_mixture.components))
        {
            std::cerr << "Default initialization of Gaussian waight (" << i + 1 << ") is not 0.2, is\n" << gaussian_mixture[i].weight << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Default initialization of Gaussian covariance (" << i + 1 << ") is 0.2: " << gaussian_mixture[i].weight << std::endl;
    }

    std::cout << "done!\n" << std::endl;



    std::cout << "Assign values to the Gaussian mixture with 5 3D components..." << std::flush;

    for (int i = 0; i < gaussian_mixture.components; ++i)
    {
        gaussian_mixture[i].mean << 1, 2, 3;

        gaussian_mixture[i].covariance << 11, 12, 13,
                                          21, 22, 23,
                                          31, 32, 33;

        gaussian_mixture[i].weight *= 2;

        if (!((gaussian_mixture[i].mean(0) == 1) &&
              (gaussian_mixture[i].mean(1) == 2) &&
              (gaussian_mixture[i].mean(2) == 3)   ))
        {
            std::cerr << "Assignment of Gaussian mean (" << i + 1 << ") failed, is\n" << gaussian_mixture[i].mean << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian mean (" << i + 1 << ") successful:\n" << gaussian_mixture[i].mean << std::endl;

        if (!((gaussian_mixture[i].covariance(0, 0) == 11) && (gaussian_mixture[i].covariance(0, 1) == 12) && (gaussian_mixture[i].covariance(0, 2) == 13) &&
              (gaussian_mixture[i].covariance(1, 0) == 21) && (gaussian_mixture[i].covariance(1, 1) == 22) && (gaussian_mixture[i].covariance(1, 2) == 23) &&
              (gaussian_mixture[i].covariance(2, 0) == 31) && (gaussian_mixture[i].covariance(2, 1) == 32) && (gaussian_mixture[i].covariance(2, 2) == 33)   ))
        {
            std::cerr << "Assignment of Gaussian covariance (" << i + 1 << ") failed, is\n" << gaussian_mixture[i].covariance << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian covariance (" << i + 1 << ") successful:\n" << gaussian_mixture[i].covariance << std::endl;

        if (!((gaussian_mixture[i].weight - 2.0 / gaussian_mixture.components) < 0.00001))
        {
            std::cerr << "Assignment of Gaussian weight (" << i + 1 << ") failed, is\n" << gaussian_mixture[i].weight << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian weight (" << i + 1 << ") successful:\n" << gaussian_mixture[i].weight << std::endl;
    }

    std::cout << "done!\n" << std::endl;


    return EXIT_SUCCESS;
}
