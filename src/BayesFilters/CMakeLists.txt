set(LIBRARY_TARGET_NAME BayesFilters)

# List of CPP (source) library files
set(${LIBRARY_TARGET_NAME}_FC_SRC
        src/FilteringContext.cpp)

set(${LIBRARY_TARGET_NAME}_FA_SRC
        src/FilteringAlgorithm.cpp
        src/KalmanFilter.cpp
        src/SIRParticleFilter.cpp
        src/UnscentedKalmanFilter.cpp)

set(${LIBRARY_TARGET_NAME}_FF_SRC
        src/AuxiliaryFunction.cpp
        src/CorrectionDecorator.cpp
        src/DrawParticle.cpp
        src/LinearSensor.cpp
        src/ParticleFilterCorrection.cpp
        src/ParticleFilterPredictionDecorator.cpp
        src/Resampling.cpp
        src/StateModelDecorator.cpp
        src/VisualCorrectionDecorator.cpp
        src/WhiteNoiseAcceleration.cpp)

set(${LIBRARY_TARGET_NAME}_SRC
        ${${LIBRARY_TARGET_NAME}_FC_SRC}
        ${${LIBRARY_TARGET_NAME}_FA_SRC}
        ${${LIBRARY_TARGET_NAME}_FF_SRC}
)

# List of HPP (header) library files
set(${LIBRARY_TARGET_NAME}_FC_HDR
        include/BayesFilters/FilteringContext.h)

set(${LIBRARY_TARGET_NAME}_FA_HDR
        include/BayesFilters/FilteringAlgorithm.h
        include/BayesFilters/KalmanFilter.h
        include/BayesFilters/SIRParticleFilter.h
        include/BayesFilters/UnscentedKalmanFilter.h)

set(${LIBRARY_TARGET_NAME}_FF_HDR
        include/BayesFilters/AuxiliaryFunction.h
        include/BayesFilters/Correction.h
        include/BayesFilters/CorrectionDecorator.h
        include/BayesFilters/DrawParticle.h
        include/BayesFilters/Initialization.h
        include/BayesFilters/LinearSensor.h
        include/BayesFilters/ObservationModel.h
        include/BayesFilters/ParticleFilterCorrection.h
        include/BayesFilters/ParticleFilterPrediction.h
        include/BayesFilters/ParticleFilterPredictionDecorator.h
        include/BayesFilters/Resampling.h
        include/BayesFilters/SigmaPointTransform.h
        include/BayesFilters/StateModel.h
        include/BayesFilters/StateModelDecorator.h
        include/BayesFilters/VisualCorrection.h
        include/BayesFilters/VisualCorrectionDecorator.h
        include/BayesFilters/VisualObservationModel.h
        include/BayesFilters/WhiteNoiseAcceleration.h)

set(${LIBRARY_TARGET_NAME}_HDR
        ${${LIBRARY_TARGET_NAME}_FC_HDR}
        ${${LIBRARY_TARGET_NAME}_FA_HDR}
        ${${LIBRARY_TARGET_NAME}_FF_HDR}
)

find_package(EIGEN3 REQUIRED)
find_package(OpenCV REQUIRED)

add_library(${LIBRARY_TARGET_NAME} ${${LIBRARY_TARGET_NAME}_SRC} ${${LIBRARY_TARGET_NAME}_HDR})

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES VERSION       ${${PROJECT_NAME}_VERSION}
                                                        PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}")

target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                         "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}>"
                                                         ${EIGEN3_INCLUDE_DIRS})

target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC ${OpenCV_LIBS})

# Specify installation targets, typology and destination folders.
install(TARGETS ${LIBRARY_TARGET_NAME}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME}" COMPONENT dev)

set_property(GLOBAL APPEND PROPERTY ${PROJECT_NAME}_TARGETS ${LIBRARY_TARGET_NAME})

# Extra options
target_compile_features(${LIBRARY_TARGET_NAME} PUBLIC cxx_nullptr cxx_constexpr)

if(${CMAKE_GENERATOR} MATCHES "Xcode")
    source_group("src\\FilteringContext"   FILES ${${LIBRARY_TARGET_NAME}_FC_SRC})
    source_group("src\\FilteringAlgorithm" FILES ${${LIBRARY_TARGET_NAME}_FA_SRC})
    source_group("src\\FilteringFunction"  FILES ${${LIBRARY_TARGET_NAME}_FF_SRC})

    source_group("include\\FilteringContext"   FILES ${${LIBRARY_TARGET_NAME}_FC_HDR})
    source_group("include\\FilteringAlgorithm" FILES ${${LIBRARY_TARGET_NAME}_FA_HDR})
    source_group("include\\FilteringFunction"  FILES ${${LIBRARY_TARGET_NAME}_FF_HDR})
endif()
