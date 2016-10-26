cmake_minimum_required(VERSION 3.1)

project(BayesFiltersLib)


### --- FILES --- ###
set(INCLUDE_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(FC_INCLIDE_FILES    ${INCLUDE_DIR}/FilteringContext/FilteringContext.h)
set(FA_INCLUDE_FILES    ${INCLUDE_DIR}/FilteringAlgorithm/FilteringAlgorithm.h
                        ${INCLUDE_DIR}/FilteringAlgorithm/KalmanFilter.h
                        ${INCLUDE_DIR}/FilteringAlgorithm/UnscentedKalmanFilter.h
                        ${INCLUDE_DIR}/FilteringAlgorithm/SIRParticleFilter.h
                        ${INCLUDE_DIR}/FilteringAlgorithm/ASIRParticleFilter.h)
set(FF_INCLUDE_FILES    ${INCLUDE_DIR}/FilteringFunction/BayesianFilteringFunction.h
                        ${INCLUDE_DIR}/FilteringFunction/KalmanFilteringFunction.h
                        ${INCLUDE_DIR}/FilteringFunction/UnscentedKalmanFilteringFunction.h
                        ${INCLUDE_DIR}/FilteringFunction/ParticleFilteringFunction.h
                        ${INCLUDE_DIR}/FilteringFunction/AuxiliaryParticleFilteringFunction.h
                        ${INCLUDE_DIR}/FilteringFunction/MultiObjectsFilteringFunction.h)

set(SOURCE_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(FC_SOURCE_FILES     ${SOURCE_DIR}/FilteringContext/FilteringContext.cpp)
set(FA_SOURCE_FILES     ${SOURCE_DIR}/FilteringAlgorithm/KalmanFilter.cpp
                        ${SOURCE_DIR}/FilteringAlgorithm/UnscentedKalmanFilter.cpp
                        ${SOURCE_DIR}/FilteringAlgorithm/SIRParticleFilter.cpp
                        ${SOURCE_DIR}/FilteringAlgorithm/ASIRParticleFilter.cpp)
set(FF_SOURCE_FILES     ${SOURCE_DIR}/FilteringFunction/KalmanFilteringFunction.cpp
                        ${SOURCE_DIR}/FilteringFunction/UnscentedKalmanFilteringFunction.cpp
                        ${SOURCE_DIR}/FilteringFunction/ParticleFilteringFunction.cpp
                        ${SOURCE_DIR}/FilteringFunction/AuxiliaryParticleFilteringFunction.cpp)


### --- INCLUDE & EXECUTABLE VARS --- ###
set(SOURCE_FILES        ${SOURCE_DIR}/main.cpp
                        ${FC_SOURCE_FILES}
                        ${FA_SOURCE_FILES}
                        ${FF_SOURCE_FILES})

set(INCLUDE_FILES       ${FC_INCLIDE_FILES}
                        ${FA_INCLUDE_FILES}
                        ${FF_INCLUDE_FILES})


### --- PACKAGES --- ###
set(LIBS)


### --- MAIN --- ###
include_directories(${INCLUDE_DIR}/FilteringContext
                    ${INCLUDE_DIR}/FilteringAlgorithm
                    ${INCLUDE_DIR}/FilteringFunction
                    $ENV{EIGEN3_INCLUDE_DIR})

add_executable(${PROJECT_NAME}          ${SOURCE_FILES} ${INCLUDE_FILES})

target_link_libraries(${PROJECT_NAME}   ${LIBS})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_nullptr cxx_constexpr)


### --- CUSTOM --- ###
if(${CMAKE_GENERATOR} MATCHES "Xcode")
    source_group("Source Files\\FilteringContext"   FILES ${FC_SOURCE_FILES})
    source_group("Source Files\\FilteringAlgorithm" FILES ${FA_SOURCE_FILES})
    source_group("Source Files\\FilteringFunction"  FILES ${FF_SOURCE_FILES})
    source_group("Header Files\\FilteringContext"   FILES ${FC_INCLIDE_FILES})
    source_group("Header Files\\FilteringAlgorithm" FILES ${FA_INCLUDE_FILES})
    source_group("Header Files\\FilteringFunction"  FILES ${FF_INCLUDE_FILES})
endif()
