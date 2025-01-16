set(LIBRARY_NAME "eurora_utils")

file(GLOB_RECURSE exception_files
    ${CMAKE_SOURCE_DIR}/src/utils/exception/*
)

file(GLOB_RECURSE logger_files
    ${CMAKE_SOURCE_DIR}/src/utils/logger/*
)

file(GLOB_RECURSE pattern_files
    ${CMAKE_SOURCE_DIR}/src/utils/pattern/*
)

file(GLOB_RECURSE utils_files
    ${CMAKE_SOURCE_DIR}/src/utils/*
)

source_group("Exception" FILES ${exception_files})
source_group("Logger" FILES ${logger_files})
source_group("Utils" FILES ${utils_files})
source_group("Pattern" FILES ${pattern_files})

find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(Boost REQUIRED)

add_library(${LIBRARY_NAME} ${exception_files} ${logger_files} ${utils_files} ${pattern_files})
add_library(eurora::eurora_utils ALIAS ${LIBRARY_NAME})

# add project options
target_link_libraries(${LIBRARY_NAME}
    PUBLIC
        ProjectOptions
        spdlog::spdlog_header_only
        fmt::fmt
        nlohmann_json::nlohmann_json
)

target_link_libraries(${LIBRARY_NAME}
    PRIVATE
        Boost::headers
        Boost::nowide
)

# Include module for GNU standard installation directories
include(GNUInstallDirs)

# Install library and executable and export as a set
install(TARGETS
    ${LIBRARY_NAME}
    EXPORT ${LIBRARY_NAME}ExportSet
)

# Install the export set
install(EXPORT ${LIBRARY_NAME}ExportSet
    FILE ${LIBRARY_NAME}Targets.cmake
    NAMESPACE ${LIBRARY_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY_NAME}
)
