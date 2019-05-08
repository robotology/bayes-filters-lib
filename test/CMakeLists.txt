#===============================================================================
#
# Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
#
# This software may be modified and distributed under the terms of the
# BSD 3-Clause license. See the accompanying LICENSE file for details.
#
#===============================================================================

option(TEST_LOG_TO_FILE "Write test logs to file" OFF)

add_subdirectory(test_DirectionalStatisticsUtils)
add_subdirectory(test_Gaussian)
add_subdirectory(test_Gaussian_Density_UVR)
add_subdirectory(test_SigmaPointUtils)
add_subdirectory(test_SIS)
add_subdirectory(test_KF)
add_subdirectory(test_UKF)
add_subdirectory(test_mixed_KF_UKF)
add_subdirectory(test_mixed_UKF_KF)
add_subdirectory(test_UPF)
add_subdirectory(test_mixed_KF_SUKF)
add_subdirectory(test_UPF_MAP)
