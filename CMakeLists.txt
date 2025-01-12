cmake_minimum_required(VERSION 3.27.0)

# Check the compiler version
include(cmake/DetectCompiler.cmake)

# Set global C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create Eurora project
project(
    Eurora
    VERSION 0.1
    DESCRIPTION "MRI Recon Framework"
    HOMEPAGE_URL "%%github.com/isqiwen/eurora%%"
    LANGUAGES CXX
)

# Set standard project settings
include(cmake/StandardProjectSettings.cmake)

# set project options
include(cmake/ProjectOptions.cmake)

# set static analysers
#include(cmake/StaticAnalysers.cmake)

# Add the src directory to the build
add_subdirectory(src)

# Enable CTest
#enable_testing()

# Add the test subdirectory to the build
#add_subdirectory(test)
