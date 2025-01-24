cmake_minimum_required(VERSION 3.27.0)

project(
    Eurora
    VERSION 0.1
    DESCRIPTION "MRI Recon Framework"
    HOMEPAGE_URL "%%github.com/isqiwen/eurora%%"
    LANGUAGES CXX
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed. Use a separate build directory.")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Tools.cmake)
include(cmake/StandardSettings.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/ProjectOptions.cmake)
include(cmake/ThirdParty/ISMRMRD.cmake)
include(cmake/ThirdParty/FindMKL.cmake)

add_cmake_format_target()
add_clang_format_target()
print_project_configuration()

add_subdirectory(src)

add_subdirectory(benchmarks)

if(${PROJECT_NAME}_ENABLE_UNIT_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()

if(${PROJECT_NAME}_ENABLE_DOXYGEN)
    include(cmake/Doxygen.cmake)
    setup_doxygen()
endif()
