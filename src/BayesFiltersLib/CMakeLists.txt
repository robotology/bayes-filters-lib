set(LIBRARY_TARGET_NAME BayesFiltersLib)

# List of CPP (source) library files
set(${LIBRARY_TARGET_NAME}_FC_SRC   src/FilteringContext/FilteringContext.cpp)
set(${LIBRARY_TARGET_NAME}_FA_SRC   src/FilteringAlgorithm/KalmanFilter.cpp
                                    src/FilteringAlgorithm/UnscentedKalmanFilter.cpp
                                    src/FilteringAlgorithm/SIRParticleFilter.cpp
                                    src/FilteringAlgorithm/ASIRParticleFilter.cpp)
set(${LIBRARY_TARGET_NAME}_FF_SRC   src/FilteringFunction/KalmanFilteringFunction.cpp
                                    src/FilteringFunction/UnscentedKalmanFilteringFunction.cpp
                                    src/FilteringFunction/ParticleFilteringFunction.cpp
                                    src/FilteringFunction/AuxiliaryParticleFilteringFunction.cpp)

set(${LIBRARY_TARGET_NAME}_SRC
        ${${LIBRARY_TARGET_NAME}_FC_SRC}
        ${${LIBRARY_TARGET_NAME}_FA_SRC}
        ${${LIBRARY_TARGET_NAME}_FF_SRC}
)

# List of HPP (header) library files
set(${LIBRARY_TARGET_NAME}_FC_HDR   include/FilteringContext/FilteringContext.h)
set(${LIBRARY_TARGET_NAME}_FA_HDR   include/FilteringAlgorithm/FilteringAlgorithm.h
                                    include/FilteringAlgorithm/KalmanFilter.h
                                    include/FilteringAlgorithm/UnscentedKalmanFilter.h
                                    include/FilteringAlgorithm/SIRParticleFilter.h
                                    include/FilteringAlgorithm/ASIRParticleFilter.h)
set(${LIBRARY_TARGET_NAME}_FF_HDR   include/FilteringFunction/BayesianFilteringFunction.h
                                    include/FilteringFunction/KalmanFilteringFunction.h
                                    include/FilteringFunction/UnscentedKalmanFilteringFunction.h
                                    include/FilteringFunction/ParticleFilteringFunction.h
                                    include/FilteringFunction/AuxiliaryParticleFilteringFunction.h
                                    include/FilteringFunction/MultiObjectsFilteringFunction.h)

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
