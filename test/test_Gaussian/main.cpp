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

        try
        {
            Gaussian(0);
        }
        catch (const std::runtime_error& e)
        {
            std::cout << "Caught error.\n" << e.what() << std::endl;

            return EXIT_FAILURE;
        }

        std::cout << "...done!\n" << std::endl;
    }


    std::cout << "[Test 2] 1D Gaussian" << std::endl;
    {
        std::cout << "Constructing a 1D Gaussian..." << std::endl;

        Gaussian gaussian_1d;

        if (!(gaussian_1d.mean().array() == 0).all())
        {
            std::cerr << "Default initialization of Gaussian mean is not zero, is\n" << gaussian_1d.mean() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Default initialization of Gaussian mean is all zero:\n" << gaussian_1d.mean() << std::endl;

        if (!(gaussian_1d.covariance().array() == 0).all())
        {
            std::cerr << "Default initialization of Gaussian covariance is not zero, is\n" << gaussian_1d.covariance() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Default initialization of Gaussian covariance is all zero:\n" << gaussian_1d.covariance() << std::endl;

        if (!(gaussian_1d.weight() == 1.0))
        {
            std::cerr << "Default initialization of Gaussian weight is not 1.0, is\n" << gaussian_1d.weight() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Default initialization of Gaussian covariance is 1.0: " << gaussian_1d.weight() << std::endl;

        std::cout << "done!\n" << std::endl;
    }


    std::cout << "[Test 3] 3D Gaussian" << std::endl;
    {
        std::cout << "Constructing a 3D Gaussian..." << std::endl;

        Gaussian gaussian_3d(3);

        if (!(gaussian_3d.mean().array() == 0).all())
        {
            std::cerr << "Default initialization of Gaussian mean is not zero, is\n" << gaussian_3d.mean() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Default initialization of Gaussian mean is all zero:\n" << gaussian_3d.mean() << std::endl;

        if (!(gaussian_3d.covariance().array() == 0).all())
        {
            std::cerr << "Default initialization of Gaussian covariance is not zero, is\n" << gaussian_3d.covariance() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Default initialization of Gaussian covariance is all zero:\n" << gaussian_3d.covariance() << std::endl;

        if (!(gaussian_3d.weight() == 1.0))
        {
            std::cerr << "Default initialization of Gaussian weight is not 1.0, is\n" << gaussian_3d.weight() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Default initialization of Gaussian weight is 1.0: " << gaussian_3d.weight() << std::endl;

        std::cout << "done!\n" << std::endl;


        std::cout << "Assign values to the Gaussian with 3D components..." << std::endl;

        gaussian_3d.mean() << 1, 2, 3;

        gaussian_3d.covariance() << 11, 12, 13,
                                    21, 22, 23,
                                    31, 32, 33;

        gaussian_3d.weight() = 0.5;

        if (!((gaussian_3d.mean(0) == 1) &&
              (gaussian_3d.mean(1) == 2) &&
              (gaussian_3d.mean(2) == 3)))
        {
            std::cerr << "Assignment of Gaussian mean failed, is\n" << gaussian_3d.mean() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian mean successful:\n" << gaussian_3d.mean() << std::endl;

        if (!((gaussian_3d.covariance(0, 0) == 11) && (gaussian_3d.covariance(0, 1) == 12) && (gaussian_3d.covariance(0, 2) == 13) &&
              (gaussian_3d.covariance(1, 0) == 21) && (gaussian_3d.covariance(1, 1) == 22) && (gaussian_3d.covariance(1, 2) == 23) &&
              (gaussian_3d.covariance(2, 0) == 31) && (gaussian_3d.covariance(2, 1) == 32) && (gaussian_3d.covariance(2, 2) == 33)))
        {
            std::cerr << "Assignment of Gaussian covariance failed, is\n" << gaussian_3d.covariance() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian covariance successful:\n" << gaussian_3d.covariance() << std::endl;

        if (!((gaussian_3d.weight() - 0.5) < 0.00001))
        {
            std::cerr << "Assignment of Gaussian weight failed, is\n" << gaussian_3d.weight() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian weight successful:\n" << gaussian_3d.weight() << std::endl;


        std::cout << "Initializing Gaussian using copy and move constructor..." << std::endl;

        Gaussian gaussian_3d_copy(gaussian_3d);

        Gaussian gaussian_3d_move(std::move(gaussian_3d_copy));

        if (!((gaussian_3d_move.mean(0) == 1) &&
              (gaussian_3d_move.mean(1) == 2) &&
              (gaussian_3d_move.mean(2) == 3)))
        {
            std::cerr << "Assignment of Gaussian mean failed, is\n" << gaussian_3d_move.mean() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian mean successful:\n" << gaussian_3d_move.mean() << std::endl;

        if (!((gaussian_3d_move.covariance(0, 0) == 11) && (gaussian_3d_move.covariance(0, 1) == 12) && (gaussian_3d_move.covariance(0, 2) == 13) &&
              (gaussian_3d_move.covariance(1, 0) == 21) && (gaussian_3d_move.covariance(1, 1) == 22) && (gaussian_3d_move.covariance(1, 2) == 23) &&
              (gaussian_3d_move.covariance(2, 0) == 31) && (gaussian_3d_move.covariance(2, 1) == 32) && (gaussian_3d_move.covariance(2, 2) == 33)))
        {
            std::cerr << "Assignment of Gaussian covariance failed, is\n" << gaussian_3d_move.covariance() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian covariance successful:\n" << gaussian_3d_move.covariance() << std::endl;

        if (!((gaussian_3d_move.weight() - 0.5) < 0.00001))
        {
            std::cerr << "Assignment of Gaussian weight failed, is\n" << gaussian_3d_move.weight() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Assignment of Gaussian weight successful:\n" << gaussian_3d_move.weight() << std::endl;
    }


    std::cout << "[Test 4] Gaussian with noise augmentation" << std::endl;
    {
        std::cout << "Constructing a Gaussian..." << std::endl;

        Gaussian gaussian(3);

        std::cout << "Assign values to the Gaussian..." << std::endl;

        gaussian.mean() << 1, 2, 3;

        gaussian.covariance() << 11, 12, 13,
                                 21, 22, 23,
                                 31, 32, 33;

        std::cout << "Augment gaussian with noise..." << std::endl;

        Eigen:: MatrixXd noise_covariance_matrix(2, 2);
        noise_covariance_matrix << 41, 42,
                                   51, 52;

        gaussian.augmentWithNoise(noise_covariance_matrix);

        if (!((gaussian.mean(0) == 1) &&
              (gaussian.mean(1) == 2) &&
              (gaussian.mean(2) == 3) &&
              (gaussian.mean(3) == 0) &&
              (gaussian.mean(4) == 0)))
        {
            std::cerr << "Mean of augmented gaussian is wrong, it is\n" << gaussian.mean() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Mean of augmented gaussian evaluated succesfully:\n" << gaussian.mean() << std::endl;

        if (!((gaussian.covariance(0, 0) == 11) && (gaussian.covariance(0, 1) == 12) && (gaussian.covariance(0, 2) == 13) && (gaussian.covariance(0, 3) == 0 ) && (gaussian.covariance(0, 4) == 0 ) &&
              (gaussian.covariance(1, 0) == 21) && (gaussian.covariance(1, 1) == 22) && (gaussian.covariance(1, 2) == 23) && (gaussian.covariance(1, 3) == 0 ) && (gaussian.covariance(1, 4) == 0 ) &&
              (gaussian.covariance(2, 0) == 31) && (gaussian.covariance(2, 1) == 32) && (gaussian.covariance(2, 2) == 33) && (gaussian.covariance(2, 3) == 0 ) && (gaussian.covariance(2, 4) == 0 ) &&
              (gaussian.covariance(3, 0) == 0 ) && (gaussian.covariance(3, 1) == 0 ) && (gaussian.covariance(3, 2) == 0 ) && (gaussian.covariance(3, 3) == 41) && (gaussian.covariance(3, 4) == 42) &&
              (gaussian.covariance(4, 0) == 0 ) && (gaussian.covariance(4, 1) == 0 ) && (gaussian.covariance(4, 2) == 0 ) && (gaussian.covariance(4, 3) == 51) && (gaussian.covariance(4, 4) == 52)))
        {
            std::cerr << "Covariance of augmented gaussian is wrong, it is\n" << gaussian.covariance() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Covariance of augmented gaussian evaluated successful:\n" << gaussian.covariance() << std::endl;
    }


    std::cout << "[Test 5] Gaussian Mixture" << std::endl;
    {
        std::cout << "Constructing a Gaussian mixture with 5 3D components..." << std::endl;

        GaussianMixture gaussian_mixture(5, 3);

        for (int i = 0; i < gaussian_mixture.components; ++i)
        {
            if (!(gaussian_mixture.mean(i).array() == 0).all())
            {
                std::cerr << "Default initialization of Gaussian mean (" << i + 1 << ") is not zero, is\n" << gaussian_mixture.mean(i) << std::endl;
                return EXIT_FAILURE;
            }
            else
                std::cout << "Default initialization of Gaussian mean (" << i + 1 << ") is all zero:\n" << gaussian_mixture.mean(i) << std::endl;

            if (!(gaussian_mixture.covariance(i).array() == 0).all())
            {
                std::cerr << "Default initialization of Gaussian covariance (" << i + 1 << ") is not zero, is\n" << gaussian_mixture.covariance(i) << std::endl;
                return EXIT_FAILURE;
            }
            else
                std::cout << "Default initialization of Gaussian covariance (" << i + 1 << ") is all zero:\n" << gaussian_mixture.covariance(i) << std::endl;

            if (!((gaussian_mixture.weight(i) - 1.0 / gaussian_mixture.components) < 0.00001))
            {
                std::cerr << "Default initialization of Gaussian weight (" << i + 1 << ") is not 0.2, is\n" << gaussian_mixture.weight(i) << std::endl;
                return EXIT_FAILURE;
            }
            else
                std::cout << "Default initialization of Gaussian covariance (" << i + 1 << ") is 0.2: " << gaussian_mixture.weight(i) << std::endl;
        }

        std::cout << "done!\n" << std::endl;


        std::cout << "Assign values to the Gaussian mixture with 5 3D components..." << std::endl;

        for (int i = 0; i < gaussian_mixture.components; ++i)
        {
            gaussian_mixture.mean(i) << 1, 2, 3;

            gaussian_mixture.covariance(i) << 11, 12, 13,
                                              21, 22, 23,
                                              31, 32, 33;

            gaussian_mixture.weight(i) *= 2;

            if (!((gaussian_mixture.mean(i, 0) == 1) &&
                  (gaussian_mixture.mean(i, 1) == 2) &&
                  (gaussian_mixture.mean(i, 2) == 3)))
            {
                std::cerr << "Assignment of Gaussian mean (" << i + 1 << ") failed, is\n" << gaussian_mixture.mean(i) << std::endl;
                return EXIT_FAILURE;
            }
            else
                std::cout << "Assignment of Gaussian mean (" << i + 1 << ") successful:\n" << gaussian_mixture.mean(i) << std::endl;

            if (!((gaussian_mixture.covariance(i, 0, 0) == 11) && (gaussian_mixture.covariance(i, 0, 1) == 12) && (gaussian_mixture.covariance(i, 0, 2) == 13) &&
                  (gaussian_mixture.covariance(i, 1, 0) == 21) && (gaussian_mixture.covariance(i, 1, 1) == 22) && (gaussian_mixture.covariance(i, 1, 2) == 23) &&
                  (gaussian_mixture.covariance(i, 2, 0) == 31) && (gaussian_mixture.covariance(i, 2, 1) == 32) && (gaussian_mixture.covariance(i, 2, 2) == 33)))
            {
                std::cerr << "Assignment of Gaussian covariance (" << i + 1 << ") failed, is\n" << gaussian_mixture.covariance(i) << std::endl;
                return EXIT_FAILURE;
            }
            else
                std::cout << "Assignment of Gaussian covariance (" << i + 1 << ") successful:\n" << gaussian_mixture.covariance(i) << std::endl;

            if (!((gaussian_mixture.weight(i) - 2.0 / gaussian_mixture.components) < 0.00001))
            {
                std::cerr << "Assignment of Gaussian weight (" << i + 1 << ") failed, is\n" << gaussian_mixture.weight(i) << std::endl;
                return EXIT_FAILURE;
            }
            else
                std::cout << "Assignment of Gaussian weight (" << i + 1 << ") successful:\n" << gaussian_mixture.weight(i) << std::endl;
        }

        std::cout << "done!\n" << std::endl;


        std::cout << "Reading values altogether to the Gaussian mixture with 5 3D components..." << std::endl;

        std::cout << "Means:\n" << gaussian_mixture.mean() << std::endl;
        std::cout << "Covariances:\n" << gaussian_mixture.covariance() << std::endl;
        std::cout << "Weights:\n" << gaussian_mixture.weight() << std::endl;

        std::cout << "done!\n" << std::endl;
    }


    std::cout << "[Test 6] Gaussian Mixture with noise augmentation" << std::endl;
    {
        std::cout << "Constructing a Gaussian mixture..." << std::endl;

        GaussianMixture mixture(2, 1, 1);

        std::cout << "Assign values to the Gaussian mixture..." << std::endl;

        mixture.mean(0) << 1, 2;

        mixture.covariance(0) << 11, 12,
                                 21, 22;

        mixture.mean(1) << 3, 4;
        mixture.covariance(1) << 31, 32,
                                 41, 42;

        std::cout << "Augment gaussian with noise..." << std::endl;

        Eigen:: MatrixXd noise_covariance_matrix(2, 2);
        noise_covariance_matrix << 51, 52,
                                   61, 62;

        mixture.augmentWithNoise(noise_covariance_matrix);

        if (!(
              /* Mean of first component. */
              (mixture.mean(0, 0) == 1) &&
              (mixture.mean(0, 1) == 2) &&
              (mixture.mean(0, 2) == 0) &&
              (mixture.mean(0, 3) == 0) &&

              /* Mean of second component. */
              (mixture.mean(1, 0) == 3) &&
              (mixture.mean(1, 1) == 4) &&
              (mixture.mean(1, 2) == 0) &&
              (mixture.mean(1, 3) == 0)))
        {
            std::cerr << "Mean of augmented gaussian mixture is wrong, it is\n" << mixture.mean() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Mean of augmented gaussian mixture evaluated succesfully:\n" << mixture.mean() << std::endl;

        if (!(
              /* Covariance of first component. */
              (mixture.covariance(0, 0, 0) == 11) && (mixture.covariance(0, 0, 1) == 12) && (mixture.covariance(0, 0, 2) == 0 ) && (mixture.covariance(0, 0, 3) == 0 ) &&
              (mixture.covariance(0, 1, 0) == 21) && (mixture.covariance(0, 1, 1) == 22) && (mixture.covariance(0, 1, 2) == 0 ) && (mixture.covariance(0, 1, 3) == 0 ) &&
              (mixture.covariance(0, 2, 0) == 0 ) && (mixture.covariance(0, 2, 1) == 0 ) && (mixture.covariance(0, 2, 2) == 51) && (mixture.covariance(0, 2, 3) == 52) &&
              (mixture.covariance(0, 3, 0) == 0 ) && (mixture.covariance(0, 3, 1) == 0 ) && (mixture.covariance(0, 3, 2) == 61) && (mixture.covariance(0, 3, 3) == 62) &&

              /* Covariance of second component. */
              (mixture.covariance(1, 0, 0) == 31) && (mixture.covariance(1, 0, 1) == 32) && (mixture.covariance(1, 0, 2) == 0 ) && (mixture.covariance(1, 0, 3) == 0 ) &&
              (mixture.covariance(1, 1, 0) == 41) && (mixture.covariance(1, 1, 1) == 42) && (mixture.covariance(1, 1, 2) == 0 ) && (mixture.covariance(1, 1, 3) == 0 ) &&
              (mixture.covariance(1, 2, 0) == 0 ) && (mixture.covariance(1, 2, 1) == 0 ) && (mixture.covariance(1, 2, 2) == 51) && (mixture.covariance(1, 2, 3) == 52) &&
              (mixture.covariance(1, 3, 0) == 0 ) && (mixture.covariance(1, 3, 1) == 0 ) && (mixture.covariance(1, 3, 2) == 61) && (mixture.covariance(1, 3, 3) == 62)))
        {
            std::cerr << "Covariance of augmented gaussian is wrong, it is\n" << mixture.covariance() << std::endl;
            return EXIT_FAILURE;
        }
        else
            std::cout << "Covariance of augmented gaussian evaluated successful:\n" << mixture.covariance() << std::endl;
        std::cout << "done!\n" << std::endl;
    }

    return EXIT_SUCCESS;
}
