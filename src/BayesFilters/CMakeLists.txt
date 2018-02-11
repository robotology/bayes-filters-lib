set(LIBRARY_TARGET_NAME BayesFilters)

# List of library header files
set(${LIBRARY_TARGET_NAME}_FC_HDR
        include/BayesFilters/FilteringContext.h)

set(${LIBRARY_TARGET_NAME}_FA_HDR
        include/BayesFilters/FilteringAlgorithm.h
        include/BayesFilters/KalmanFilter.h
        include/BayesFilters/ParticleFilter.h
        include/BayesFilters/SIS.h
        include/BayesFilters/UnscentedKalmanFilter.h)

set(${LIBRARY_TARGET_NAME}_FF_HDR
        include/BayesFilters/AuxiliaryFunction.h
        include/BayesFilters/DrawParticles.h
        include/BayesFilters/ExogenousModel.h
        include/BayesFilters/Initialization.h
        include/BayesFilters/LinearSensor.h
        include/BayesFilters/ObservationModel.h
        include/BayesFilters/ObservationModelDecorator.h
        include/BayesFilters/PFCorrection.h
        include/BayesFilters/PFCorrectionDecorator.h
        include/BayesFilters/PFPrediction.h
        include/BayesFilters/PFPredictionDecorator.h
        include/BayesFilters/PFVisualCorrection.h
        include/BayesFilters/PFVisualCorrectionDecorator.h
        include/BayesFilters/Resampling.h
        include/BayesFilters/ResamplingWithPrior.h
        include/BayesFilters/SPCorrection.h
        include/BayesFilters/SPPrediction.h
        include/BayesFilters/StateModel.h
        include/BayesFilters/StateModelDecorator.h
        include/BayesFilters/UpdateParticles.h
        include/BayesFilters/VisualObservationModel.h
        include/BayesFilters/VisualParticleFilter.h
        include/BayesFilters/WhiteNoiseAcceleration.h)

set(${LIBRARY_TARGET_NAME}_FU_HDR
        include/BayesFilters/EstimatesExtraction.h
        include/BayesFilters/Gaussian.h
        include/BayesFilters/GaussianMixture.h
        include/BayesFilters/HistoryBuffer.h
        include/BayesFilters/Particle.h
        include/BayesFilters/ParticleSet.h
        include/BayesFilters/sigmapointutils.h)

set(${LIBRARY_TARGET_NAME}_HDR
        ${${LIBRARY_TARGET_NAME}_FC_HDR}
        ${${LIBRARY_TARGET_NAME}_FA_HDR}
        ${${LIBRARY_TARGET_NAME}_FF_HDR}
        ${${LIBRARY_TARGET_NAME}_FU_HDR}
)

# List of library source files
set(${LIBRARY_TARGET_NAME}_FC_SRC
        src/FilteringContext.cpp)

set(${LIBRARY_TARGET_NAME}_FA_SRC
        src/FilteringAlgorithm.cpp
        src/KalmanFilter.cpp
        src/ParticleFilter.cpp
        src/SIS.cpp
        src/UnscentedKalmanFilter.cpp)

set(${LIBRARY_TARGET_NAME}_FF_SRC
        src/AuxiliaryFunction.cpp
        src/DrawParticles.cpp
        src/LinearSensor.cpp
        src/ObservationModelDecorator.cpp
        src/PFCorrection.cpp
        src/PFCorrectionDecorator.cpp
        src/PFPrediction.cpp
        src/PFPredictionDecorator.cpp
        src/PFVisualCorrection.cpp
        src/PFVisualCorrectionDecorator.cpp
        src/Resampling.cpp
        src/ResamplingWithPrior.cpp
        src/SPCorrection.cpp
        src/SPPrediction.cpp
        src/StateModelDecorator.cpp
        src/UpdateParticles.cpp
        src/VisualParticleFilter.cpp
        src/WhiteNoiseAcceleration.cpp)

set(${LIBRARY_TARGET_NAME}_FU_SRC
        src/EstimatesExtraction.cpp
        src/Gaussian.cpp
        src/GaussianMixture.cpp
        src/HistoryBuffer.cpp
        src/Particle.cpp
        src/ParticleSet.cpp
        src/sigmapointutils.cpp)

set(${LIBRARY_TARGET_NAME}_SRC
        ${${LIBRARY_TARGET_NAME}_FC_SRC}
        ${${LIBRARY_TARGET_NAME}_FA_SRC}
        ${${LIBRARY_TARGET_NAME}_FF_SRC}
        ${${LIBRARY_TARGET_NAME}_FU_SRC}
)

find_package(Threads REQUIRED)
find_package(Eigen3 QUIET CONFIG)
if(NOT EIGEN3_FOUND)
    find_package(Eigen3 REQUIRED)
endif()
find_package(OpenCV REQUIRED)

add_library(${LIBRARY_TARGET_NAME} ${${LIBRARY_TARGET_NAME}_SRC} ${${LIBRARY_TARGET_NAME}_HDR})

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES VERSION       ${${PROJECT_NAME}_VERSION}
                                                        PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}")

target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                         "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
if(NOT TARGET Eigen3)
    target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC ${EIGEN3_INCLUDE_DIR})

    target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC ${OpenCV_LIBS} Threads::Threads ${CMAKE_THREAD_LIBS_INIT})
else()
    target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC ${OpenCV_LIBS} Eigen3::Eigen Threads::Threads ${CMAKE_THREAD_LIBS_INIT})
endif()

# Specify installation targets, typology and destination folders.
install(TARGETS ${LIBRARY_TARGET_NAME}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME}" COMPONENT dev)

set_property(GLOBAL APPEND PROPERTY ${PROJECT_NAME}_TARGETS ${LIBRARY_TARGET_NAME})

if(${CMAKE_GENERATOR} MATCHES "Xcode")
    source_group("include\\${LIBRARY_TARGET_NAME}\\FilteringContext"   FILES ${${LIBRARY_TARGET_NAME}_FC_HDR})
    source_group("include\\${LIBRARY_TARGET_NAME}\\FilteringAlgorithm" FILES ${${LIBRARY_TARGET_NAME}_FA_HDR})
    source_group("include\\${LIBRARY_TARGET_NAME}\\FilteringFunction"  FILES ${${LIBRARY_TARGET_NAME}_FF_HDR})
    source_group("include\\${LIBRARY_TARGET_NAME}\\FilteringUtility"   FILES ${${LIBRARY_TARGET_NAME}_FU_HDR})

    source_group("src\\FilteringContext"                               FILES ${${LIBRARY_TARGET_NAME}_FC_SRC})
    source_group("src\\FilteringAlgorithm"                             FILES ${${LIBRARY_TARGET_NAME}_FA_SRC})
    source_group("src\\FilteringFunction"                              FILES ${${LIBRARY_TARGET_NAME}_FF_SRC})
    source_group("src\\FilteringUtility"                               FILES ${${LIBRARY_TARGET_NAME}_FU_SRC})
endif()
