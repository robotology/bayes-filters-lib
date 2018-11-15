#include <iostream>
#include <thread>

#include <BayesFilters/Gaussian.h>
#include <BayesFilters/GaussianMixture.h>

using namespace bfl;


int main()
{
    std::cout << "[Test 1] Empty Gaussian" << std::endl;
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
    }


    std::cout << "[Test 2] 1D Gaussian" << std::endl;
    {
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
            std::cerr << "Default initialization of Gaussian weight is not 1.0, is\n" << gaussian_1d.weight << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Default initialization of Gaussian covariance is 1.0: " << gaussian_1d.weight << std::endl;

        std::cout << "done!\n" << std::endl;
    }


    std::cout << "[Test 3] 3D Gaussian" << std::endl;
    {
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
            std::cerr << "Default initialization of Gaussian weight is not 1.0, is\n" << gaussian_3d.weight << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Default initialization of Gaussian covariance is 1.0: " << gaussian_3d.weight << std::endl;

        std::cout << "done!\n" << std::endl;


        std::cout << "Assign values to the Gaussian with 3D components..." << std::endl;

        gaussian_3d.mean << 1, 2, 3;

        gaussian_3d.covariance << 11, 12, 13,
                                  21, 22, 23,
                                  31, 32, 33;

        gaussian_3d.weight = 0.5;

        if (!((gaussian_3d.mean(0) == 1) &&
              (gaussian_3d.mean(1) == 2) &&
              (gaussian_3d.mean(2) == 3)))
        {
            std::cerr << "Assignment of Gaussian mean failed, is\n" << gaussian_3d.mean << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian mean successful:\n" << gaussian_3d.mean << std::endl;

        if (!((gaussian_3d.covariance(0, 0) == 11) && (gaussian_3d.covariance(0, 1) == 12) && (gaussian_3d.covariance(0, 2) == 13) &&
              (gaussian_3d.covariance(1, 0) == 21) && (gaussian_3d.covariance(1, 1) == 22) && (gaussian_3d.covariance(1, 2) == 23) &&
              (gaussian_3d.covariance(2, 0) == 31) && (gaussian_3d.covariance(2, 1) == 32) && (gaussian_3d.covariance(2, 2) == 33)))
        {
            std::cerr << "Assignment of Gaussian covariance failed, is\n" << gaussian_3d.covariance << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian covariance successful:\n" << gaussian_3d.covariance << std::endl;

        if (!((gaussian_3d.weight - 0.5) < 0.00001))
        {
            std::cerr << "Assignment of Gaussian weight failed, is\n" << gaussian_3d.weight << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian weight successful:\n" << gaussian_3d.weight << std::endl;


        std::cout << "Initializing Gaussian using copy and move constructor..." << std::endl;

        Gaussian gaussian_3d_copy(gaussian_3d);

        Gaussian gaussian_3d_move(std::move(gaussian_3d_copy));

        if (!((gaussian_3d_move.mean(0) == 1) &&
              (gaussian_3d_move.mean(1) == 2) &&
              (gaussian_3d_move.mean(2) == 3)))
        {
            std::cerr << "Assignment of Gaussian mean failed, is\n" << gaussian_3d_move.mean << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian mean successful:\n" << gaussian_3d_move.mean << std::endl;

        if (!((gaussian_3d_move.covariance(0, 0) == 11) && (gaussian_3d_move.covariance(0, 1) == 12) && (gaussian_3d_move.covariance(0, 2) == 13) &&
              (gaussian_3d_move.covariance(1, 0) == 21) && (gaussian_3d_move.covariance(1, 1) == 22) && (gaussian_3d_move.covariance(1, 2) == 23) &&
              (gaussian_3d_move.covariance(2, 0) == 31) && (gaussian_3d_move.covariance(2, 1) == 32) && (gaussian_3d_move.covariance(2, 2) == 33)))
        {
            std::cerr << "Assignment of Gaussian covariance failed, is\n" << gaussian_3d_move.covariance << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian covariance successful:\n" << gaussian_3d_move.covariance << std::endl;

        if (!((gaussian_3d_move.weight - 0.5) < 0.00001))
        {
            std::cerr << "Assignment of Gaussian weight failed, is\n" << gaussian_3d_move.weight << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian weight successful:\n" << gaussian_3d_move.weight << std::endl;
    }


    std::cout << "[Test 4] Gaussian Mixture" << std::endl;
    {
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

            if (!((gaussian_mixture[i].weight - 1.0 / gaussian_mixture.components) < 0.00001))
            {
                std::cerr << "Default initialization of Gaussian weight (" << i + 1 << ") is not 0.2, is\n" << gaussian_mixture[i].weight << std::endl;
                return EXIT_FAILURE;
            }
            else
                std::cout << "Default initialization of Gaussian covariance (" << i + 1 << ") is 0.2: " << gaussian_mixture[i].weight << std::endl;
        }

        std::cout << "done!\n" << std::endl;


        std::cout << "Assign values to the Gaussian mixture with 5 3D components..." << std::endl;

        for (int i = 0; i < gaussian_mixture.components; ++i)
        {
            gaussian_mixture[i].mean << 1, 2, 3;

            gaussian_mixture[i].covariance << 11, 12, 13,
                                              21, 22, 23,
                                              31, 32, 33;

            gaussian_mixture[i].weight *= 2;

            if (!((gaussian_mixture[i].mean(0) == 1) &&
                  (gaussian_mixture[i].mean(1) == 2) &&
                  (gaussian_mixture[i].mean(2) == 3)))
            {
                std::cerr << "Assignment of Gaussian mean (" << i + 1 << ") failed, is\n" << gaussian_mixture[i].mean << std::endl;
                return EXIT_FAILURE;
            }
            else
                std::cout << "Assignment of Gaussian mean (" << i + 1 << ") successful:\n" << gaussian_mixture[i].mean << std::endl;

            if (!((gaussian_mixture[i].covariance(0, 0) == 11) && (gaussian_mixture[i].covariance(0, 1) == 12) && (gaussian_mixture[i].covariance(0, 2) == 13) &&
                  (gaussian_mixture[i].covariance(1, 0) == 21) && (gaussian_mixture[i].covariance(1, 1) == 22) && (gaussian_mixture[i].covariance(1, 2) == 23) &&
                  (gaussian_mixture[i].covariance(2, 0) == 31) && (gaussian_mixture[i].covariance(2, 1) == 32) && (gaussian_mixture[i].covariance(2, 2) == 33)))
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


        std::cout << "Reading values altogether to the Gaussian mixture with 5 3D components..." << std::endl;

        std::cout << "Means:\n" << gaussian_mixture.mean << std::endl;
        std::cout << "Covariances:\n" << gaussian_mixture.covariance << std::endl;
        std::cout << "Weights:\n" << gaussian_mixture.weight << std::endl;

        std::cout << "done!\n" << std::endl;
    }


    std::cout << "[Test 5] GaussianRef" << std::endl;
    {
        std::cout << "Maniupulation of a Gaussian mixture with 5 3D components using GaussianRef..." << std::endl;

        GaussianMixture gaussian_mixture_ref(5, 3);

        for (int i = 0; i < gaussian_mixture_ref.components; ++i)
        {
            GaussianRef gaussian = gaussian_mixture_ref[i];

            gaussian.mean << 1, 2, 3;

            std::cout << "Testing GaussianRef copy constructor." << std::endl;

            GaussianRef gaussian_copy = gaussian;

            gaussian_copy.covariance << 11, 12, 13,
                                        21, 22, 23,
                                        31, 32, 33;

            std::cout << "Testing GaussianRef move constructor." << std::endl;

            GaussianRef gaussian_move = std::move(gaussian_copy);

            gaussian_move.weight *= 2;

            if (!((gaussian.mean(0) == 1) && (gaussian_move.mean(0) == 1) &&
                  (gaussian.mean(1) == 2) && (gaussian_move.mean(1) == 2) &&
                  (gaussian.mean(2) == 3) && (gaussian_move.mean(2) == 3)))
            {
                std::cerr << "Assignment of Gaussian mean (" << i + 1 << ") failed, is\n" << gaussian_move.mean << std::endl;
                return EXIT_FAILURE;
            }
            else
                std::cout << "Assignment of Gaussian mean (" << i + 1 << ") successful:\n" << gaussian_move.mean << std::endl;

            if (!((gaussian.covariance(0, 0) == 11)      && (gaussian.covariance(0, 1) == 12)      && (gaussian .covariance(0, 2) == 13)     &&
                  (gaussian.covariance(1, 0) == 21)      && (gaussian.covariance(1, 1) == 22)      && (gaussian .covariance(1, 2) == 23)     &&
                  (gaussian.covariance(2, 0) == 31)      && (gaussian.covariance(2, 1) == 32)      && (gaussian .covariance(2, 2) == 33)     &&
                  (gaussian_move.covariance(0, 0) == 11) && (gaussian_move.covariance(0, 1) == 12) && (gaussian_move.covariance(0, 2) == 13) &&
                  (gaussian_move.covariance(1, 0) == 21) && (gaussian_move.covariance(1, 1) == 22) && (gaussian_move.covariance(1, 2) == 23) &&
                  (gaussian_move.covariance(2, 0) == 31) && (gaussian_move.covariance(2, 1) == 32) && (gaussian_move.covariance(2, 2) == 33)))
            {
                std::cerr << "Assignment of Gaussian covariance (" << i + 1 << ") failed, is\n" << gaussian_move.covariance << std::endl;
                return EXIT_FAILURE;
            }
            else
                std::cout << "Assignment of Gaussian covariance (" << i + 1 << ") successful:\n" << gaussian_move.covariance << std::endl;

            if (!(((gaussian.weight      - 2.0 / gaussian_mixture_ref.components) < 0.00001) &&
                  ((gaussian_move.weight - 2.0 / gaussian_mixture_ref.components) < 0.00001)))
            {
                std::cerr << "Assignment of Gaussian weight (" << i + 1 << ") failed, is\n" << gaussian_move.weight << std::endl;
                return EXIT_FAILURE;
            }
            else
                std::cout << "Assignment of Gaussian weight (" << i + 1 << ") successful:\n" << gaussian_move.weight << std::endl;
        }

        std::cout << "done!\n" << std::endl;


        std::cout << "Reading values altogether to the Gaussian mixture with 5 3D components..." << std::endl;

        std::cout << "Means:\n" << gaussian_mixture_ref.mean << std::endl;
        std::cout << "Covariances:\n" << gaussian_mixture_ref.covariance << std::endl;
        std::cout << "Weights:\n" << gaussian_mixture_ref.weight << std::endl;

        std::cout << "done!\n" << std::endl;
    }


    std::cout << "[Test 6] GaussianConstRef" << std::endl;
    {
        std::cout << "Maniupulation of a constant Gaussian mixture with 5 3D components using GaussianRef..." << std::endl;

        const GaussianMixture gaussian_mixture_const_ref(5, 3);

        for (int i = 0; i < gaussian_mixture_const_ref.components; ++i)
        {
            GaussianConstRef gaussian = gaussian_mixture_const_ref[i];

            std::cout << "Testing GaussianConstRef copy constructor." << std::endl;

            GaussianConstRef gaussian_copy = gaussian;

            std::cout << "Testing GaussianConstRef move constructor." << std::endl;

            GaussianConstRef gaussian_move = std::move(gaussian_copy);

            if (!((gaussian.mean.array() == 0).all() && (gaussian_move.mean.array() == 0).all()))
            {
                std::cerr << "Default initialization of Gaussian mixture mean (" << i + 1 << ") is not zero, is\n" << gaussian_move.mean << std::endl;
                return EXIT_FAILURE;
            }
            else
                std::cout << "Default initialization of Gaussian mixture mean (" << i + 1 << ") is all zero:\n" << gaussian_move.mean << std::endl;

            if (!((gaussian.covariance.array() == 0).all() && (gaussian_move.covariance.array() == 0).all()))
            {
                std::cerr << "Default initialization of Gaussian mixture covariance (" << i + 1 << ") is not zero, is\n" << gaussian_move.covariance << std::endl;
                return EXIT_FAILURE;
            }
            else
                std::cout << "Default initialization of Gaussian mixture covariance (" << i + 1 << ") is all zero:\n" << gaussian_move.covariance << std::endl;

            if (!(((gaussian.weight      - 2.0 / gaussian_mixture_const_ref.components) < 0.00001) &&
                  ((gaussian_move.weight - 2.0 / gaussian_mixture_const_ref.components) < 0.00001)))
            {
                std::cerr << "Default initialization of Gaussian mixture weight (" << i + 1 << ") is not 0.2, is\n" << gaussian_move.weight << std::endl;
                return EXIT_FAILURE;
            }
            else
                std::cout << "Default initialization of Gaussian covariance (" << i + 1 << ") is 0.2: " << gaussian_move.weight << std::endl;
        }

        std::cout << "done!\n" << std::endl;


        std::cout << "Reading values altogether to the Gaussian mixture with 5 3D components..." << std::endl;

        std::cout << "Means:\n" << gaussian_mixture_const_ref.mean << std::endl;
        std::cout << "Covariances:\n" << gaussian_mixture_const_ref.covariance << std::endl;
        std::cout << "Weights:\n" << gaussian_mixture_const_ref.weight << std::endl;

        std::cout << "done!\n" << std::endl;
    }


    return EXIT_SUCCESS;
}
