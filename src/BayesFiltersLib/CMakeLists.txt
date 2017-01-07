set(LIBRARY_TARGET_NAME BayesFiltersLib)

# List of CPP (source) library files
set(${LIBRARY_TARGET_NAME}_FC_SRC   src/FilteringContext/FilteringContext.cpp)
set(${LIBRARY_TARGET_NAME}_FA_SRC   src/FilteringAlgorithm/ASIRParticleFilter.cpp
                                    src/FilteringAlgorithm/KalmanFilter.cpp
                                    src/FilteringAlgorithm/SIRParticleFilter.cpp
                                    src/FilteringAlgorithm/UnscentedKalmanFilter.cpp)
set(${LIBRARY_TARGET_NAME}_FF_SRC   src/FilteringFunction/AuxiliaryFunction.cpp
                                    src/FilteringFunction/Correction.cpp
                                    src/FilteringFunction/Initialization.cpp
                                    src/FilteringFunction/ObservationModel.cpp
                                    src/FilteringFunction/Prediction.cpp
                                    src/FilteringFunction/Resampling.cpp
                                    src/FilteringFunction/SigmaPointTransform.cpp
                                    src/FilteringFunction/StateModel.cpp

                                    src/FilteringFunction_old/KalmanFilteringFunction.cpp
                                    src/FilteringFunction_old/UnscentedKalmanFilteringFunction.cpp
                                    src/FilteringFunction_old/ParticleFilteringFunction.cpp
                                    src/FilteringFunction_old/AuxiliaryParticleFilteringFunction.cpp)

set(${LIBRARY_TARGET_NAME}_SRC
        ${${LIBRARY_TARGET_NAME}_FC_SRC}
        ${${LIBRARY_TARGET_NAME}_FA_SRC}
        ${${LIBRARY_TARGET_NAME}_FF_SRC}
)

# List of HPP (header) library files
set(${LIBRARY_TARGET_NAME}_FC_HDR   include/FilteringContext/FilteringContext.h)
set(${LIBRARY_TARGET_NAME}_FA_HDR   include/FilteringAlgorithm/ASIRParticleFilter.h
                                    include/FilteringAlgorithm/FilteringAlgorithm.h
                                    include/FilteringAlgorithm/KalmanFilter.h
                                    include/FilteringAlgorithm/SIRParticleFilter.h
                                    include/FilteringAlgorithm/UnscentedKalmanFilter.h)
set(${LIBRARY_TARGET_NAME}_FF_HDR   include/FilteringFunction/AuxiliaryFunction.h
                                    include/FilteringFunction/Correction.h
                                    include/FilteringFunction/Initialization.h
                                    include/FilteringFunction/ObservationModel.h
                                    include/FilteringFunction/Prediction.h
                                    include/FilteringFunction/Resampling.h
                                    include/FilteringFunction/SigmaPointTransform.h
                                    include/FilteringFunction/StateModel.h

                                    include/FilteringFunction_old/BayesianFilteringFunction.h
                                    include/FilteringFunction_old/KalmanFilteringFunction.h
                                    include/FilteringFunction_old/UnscentedKalmanFilteringFunction.h
                                    include/FilteringFunction_old/ParticleFilteringFunction.h
                                    include/FilteringFunction_old/AuxiliaryParticleFilteringFunction.h
                                    include/FilteringFunction_old/MultiObjectsFilteringFunction.h)

set(${LIBRARY_TARGET_NAME}_HDR
        ${${LIBRARY_TARGET_NAME}_FC_HDR}
        ${${LIBRARY_TARGET_NAME}_FA_HDR}
        ${${LIBRARY_TARGET_NAME}_FF_HDR}
)

find_package(EIGEN3 REQUIRED)

add_library(${LIBRARY_TARGET_NAME} ${${LIBRARY_TARGET_NAME}_SRC} ${${LIBRARY_TARGET_NAME}_HDR})

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES VERSION       ${${PROJECT_NAME}_VERSION}
                                                        PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}")

target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}>"
                                                            ${EIGEN3_INCLUDE_DIRS})

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
