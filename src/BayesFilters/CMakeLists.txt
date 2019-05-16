#===============================================================================
#
# Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
#
# This software may be modified and distributed under the terms of the
# BSD 3-Clause license. See the accompanying LICENSE file for details.
#
#===============================================================================

set(LIBRARY_TARGET_NAME BayesFilters)

# List of library header files
set(${LIBRARY_TARGET_NAME}_FA_HDR
        include/BayesFilters/FilteringAlgorithm.h
        include/BayesFilters/GaussianFilter.h
        include/BayesFilters/ParticleFilter.h
        include/BayesFilters/SIS.h
)

set(${LIBRARY_TARGET_NAME}_FF_HDR
        include/BayesFilters/AdditiveMeasurementModel.h
        include/BayesFilters/AdditiveStateModel.h
        include/BayesFilters/Agent.h
        include/BayesFilters/BootstrapCorrection.h
        include/BayesFilters/DrawParticles.h
        include/BayesFilters/ExogenousModel.h
        include/BayesFilters/ExogenousProcess.h
        include/BayesFilters/GaussianCorrection.h
        include/BayesFilters/GaussianLikelihood.h
        include/BayesFilters/GaussianMixturePrediction.h
        include/BayesFilters/GaussianInitialization.h
        include/BayesFilters/GaussianPrediction.h
        include/BayesFilters/GPFCorrection.h
        include/BayesFilters/GPFPrediction.h
        include/BayesFilters/InitSurveillanceAreaGrid.h
        include/BayesFilters/KFCorrection.h
        include/BayesFilters/KFPrediction.h
        include/BayesFilters/LikelihoodModel.h
        include/BayesFilters/LinearMeasurementModel.h
        include/BayesFilters/LinearModel.h
        include/BayesFilters/LinearStateModel.h
        include/BayesFilters/LTIMeasurementModel.h
        include/BayesFilters/LTIStateModel.h
        include/BayesFilters/MeasurementModel.h
        include/BayesFilters/ParticleSetInitialization.h
        include/BayesFilters/PFCorrection.h
        include/BayesFilters/PFPrediction.h
        include/BayesFilters/Resampling.h
        include/BayesFilters/ResamplingWithPrior.h
        include/BayesFilters/SimulatedLinearSensor.h
        include/BayesFilters/SimulatedStateModel.h
        include/BayesFilters/Skipper.h
        include/BayesFilters/Skippable.h
        include/BayesFilters/StateModel.h
        include/BayesFilters/StateProcess.h
        include/BayesFilters/SUKFCorrection.h
        # include/BayesFilters/TimeDecreasingDynamics.h
        include/BayesFilters/UKFCorrection.h
        include/BayesFilters/UKFPrediction.h
        include/BayesFilters/WhiteNoiseAcceleration.h
)

set(${LIBRARY_TARGET_NAME}_FU_HDR
        include/BayesFilters/any.h
        include/BayesFilters/directional_statistics.h
        include/BayesFilters/Data.h
        include/BayesFilters/EstimatesExtraction.h
        include/BayesFilters/Gaussian.h
        include/BayesFilters/GaussianMixture.h
        include/BayesFilters/HistoryBuffer.h
        include/BayesFilters/Logger.h
        include/BayesFilters/ParticleSet.h
        include/BayesFilters/sigma_point.h
        include/BayesFilters/utils.h
)

set(${LIBRARY_TARGET_NAME}_HDR
        ${${LIBRARY_TARGET_NAME}_FA_HDR}
        ${${LIBRARY_TARGET_NAME}_FF_HDR}
        ${${LIBRARY_TARGET_NAME}_FU_HDR}
)

# List of library source files
set(${LIBRARY_TARGET_NAME}_FA_SRC
        src/FilteringAlgorithm.cpp
        src/GaussianFilter.cpp
        src/ParticleFilter.cpp
        src/SIS.cpp
)

set(${LIBRARY_TARGET_NAME}_FF_SRC
        src/AdditiveStateModel.cpp
        src/Agent.cpp
        src/BootstrapCorrection.cpp
        src/DrawParticles.cpp
        src/ExogenousModel.cpp
        src/GaussianCorrection.cpp
        src/GaussianLikelihood.cpp
        src/GaussianPrediction.cpp
        src/GPFCorrection.cpp
        src/GPFPrediction.cpp
        src/InitSurveillanceAreaGrid.cpp
        src/KFCorrection.cpp
        src/KFPrediction.cpp
        src/LinearMeasurementModel.cpp
        src/LinearModel.cpp
        src/LinearStateModel.cpp
        src/LTIMeasurementModel.cpp
        src/LTIStateModel.cpp
        src/MeasurementModel.cpp
        src/PFCorrection.cpp
        src/PFPrediction.cpp
        src/Resampling.cpp
        src/ResamplingWithPrior.cpp
        src/SimulatedLinearSensor.cpp
        src/SimulatedStateModel.cpp
        src/StateModel.cpp
        src/SUKFCorrection.cpp
        # src/TimeDecreasingDynamics.cpp
        src/UKFCorrection.cpp
        src/UKFPrediction.cpp
        src/WhiteNoiseAcceleration.cpp
)

set(${LIBRARY_TARGET_NAME}_FU_SRC
        src/directional_statistics.cpp
        src/EstimatesExtraction.cpp
        src/Gaussian.cpp
        src/GaussianMixture.cpp
        src/HistoryBuffer.cpp
        src/Logger.cpp
        src/ParticleSet.cpp
        src/sigma_point.cpp
)

set(${LIBRARY_TARGET_NAME}_SRC
        ${${LIBRARY_TARGET_NAME}_FA_SRC}
        ${${LIBRARY_TARGET_NAME}_FF_SRC}
        ${${LIBRARY_TARGET_NAME}_FU_SRC}
)

find_package(Threads REQUIRED)
find_package(Eigen3 QUIET CONFIG)
if(NOT EIGEN3_FOUND)
    find_package(Eigen3 REQUIRED)
endif()

add_library(${LIBRARY_TARGET_NAME} ${${LIBRARY_TARGET_NAME}_SRC} ${${LIBRARY_TARGET_NAME}_HDR})

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES VERSION       ${${PROJECT_NAME}_VERSION}
                                                        PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}")

target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                         "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
if(NOT TARGET Eigen3)
    target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC ${EIGEN3_INCLUDE_DIR})

    target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC Threads::Threads ${CMAKE_THREAD_LIBS_INIT})
else()
    target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC Eigen3::Eigen Threads::Threads ${CMAKE_THREAD_LIBS_INIT})
endif()

# Specify installation targets, typology and destination folders.
install(TARGETS ${LIBRARY_TARGET_NAME}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME}" COMPONENT dev
)

set_property(GLOBAL APPEND PROPERTY ${PROJECT_NAME}_TARGETS ${LIBRARY_TARGET_NAME})

# if(${CMAKE_GENERATOR} MATCHES "Xcode")
#     source_group("include\\${LIBRARY_TARGET_NAME}\\FilteringAlgorithm" FILES ${${LIBRARY_TARGET_NAME}_FA_HDR})
#     source_group("include\\${LIBRARY_TARGET_NAME}\\FilteringFunction"  FILES ${${LIBRARY_TARGET_NAME}_FF_HDR})
#     source_group("include\\${LIBRARY_TARGET_NAME}\\FilteringUtility"   FILES ${${LIBRARY_TARGET_NAME}_FU_HDR})
#
#     source_group("src\\FilteringAlgorithm"                             FILES ${${LIBRARY_TARGET_NAME}_FA_SRC})
#     source_group("src\\FilteringFunction"                              FILES ${${LIBRARY_TARGET_NAME}_FF_SRC})
#     source_group("src\\FilteringUtility"                               FILES ${${LIBRARY_TARGET_NAME}_FU_SRC})
# endif()
