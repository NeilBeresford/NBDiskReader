# ----------------------------------------------------------------------------------
# Cmake - instructions for general testing of modern C++
# Note - will do documentation later
# ----------------------------------------------------------------------------------


# Check for the mimimum CMake version
cmake_minimum_required(VERSION 3.27.0)

# Set the project name and version
project(ModernCppTests VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

# Support for external libraries
include(ExternalProject)

# Git is needed for the external projects
find_package(Git REQUIRED)

# Doctest for unit testing
ExternalProject_Add(
    doctest
    PREFIX ${CMAKE_BINARY_DIR}/doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

# Include the subdirectories for the tests
add_subdirectory(NBLibrary)
add_subdirectory(NBDiskReader)
# Include the uint test projects
add_subdirectory(TestNBLibrary)
add_subdirectory(TestNBDiskReader)


# End of the CMakeLists.txt file

