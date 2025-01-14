cmake_minimum_required(VERSION 3.27.0)

project(
    Eurora
    VERSION 0.1
    DESCRIPTION "MRI Recon Framework"
    HOMEPAGE_URL "%%github.com/isqiwen/eurora%%"
    LANGUAGES CXX
)

#
# Set project options
#

include(cmake/StandardSettings.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/Utils.cmake)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed. Use a separate build directory.")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

print_project_configuration()

include(cmake/ProjectOptions.cmake)


add_subdirectory(src)

# if(${PROJECT_NAME}_ENABLE_UNIT_TESTING)
#     add_subdirectory(test)
# endif()

if(${PROJECT_NAME}_ENABLE_DOXYGEN)
    include(cmake/Doxygen.cmake)
    setup_doxygen()
endif()
