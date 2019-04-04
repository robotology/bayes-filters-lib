#===============================================================================
#
# Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
#
# This software may be modified and distributed under the terms of the
# BSD 3-Clause license. See the accompanying LICENSE file for details.
#
#===============================================================================

cmake_minimum_required(VERSION 3.5)

# This variable should contain the argument of the project call in the main CMakeLists.txt
set(ORIGINAL_PROJECT_NAME "BayesFilters")
project(${ORIGINAL_PROJECT_NAME}Docs)

include(GNUInstallDirs)

find_package(Doxygen QUIET)

if(DOXYGEN_FOUND)
    get_filename_component(ORIGINAL_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)

    # This variables are used inside the Doxyfile.in template
    set(DOXYGEN_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE STRING "Build directory for Doxygen html output (absolute).")
    set(DOXYGEN_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/${ORIGINAL_PROJECT_NAME}" CACHE STRING "Installation directory for Doxygen html output (relative to CMAKE_INSTALL_PREFIX).")
    set(DOXYGEN_SOURCE_DIR "${ORIGINAL_PROJECT_DIR}/src")
    set(DOXYGEN_DOC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    set(DOXYGEN_EXAMPLE_DIR "${ORIGINAL_PROJECT_DIR}/test")

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # The dox target will generate the doxygen documentation in ${DOXYGEN_BUILD_DIR}
    add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                          COMMENT "Generating doxygen documentation in ${DOXYGEN_BUILD_DIR}")

    # Note: the install will install the documentation if present,
    # but it will not trigger a regeneration of the documentation if it is outdated
    install(DIRECTORY ${DOXYGEN_BUILD_DIR}/html ${DOXYGEN_BUILD_DIR}/latex
            DESTINATION ${DOXYGEN_INSTALL_DIR}
            OPTIONAL)

endif()
