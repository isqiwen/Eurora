# Create anagram library

set(LIBRARY_NAME "Common")

file(GLOB_RECURSE exception_files
    ${CMAKE_SOURCE_DIR}/src/common/exception/*
    ${CMAKE_SOURCE_DIR}/src/internal/common/exception/*
)

file(GLOB_RECURSE logger_files
    ${CMAKE_SOURCE_DIR}/src/internal/common/logger/*
)

file(GLOB_RECURSE utils_files
    ${CMAKE_SOURCE_DIR}/src/common/utils/*
    ${CMAKE_SOURCE_DIR}/src/internal/common/utils/*
)

source_group("Exception" FILES ${exception_files})
source_group("Logger" FILES ${logger_files})
source_group("Utils" FILES ${utils_files})

find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

add_library(${LIBRARY_NAME} ${exception_files} ${logger_files} ${utils_files})

# add project options
target_link_libraries(${LIBRARY_NAME}
    PRIVATE
        ProjectOptions
        nlohmann_json::nlohmann_json
        spdlog::spdlog_header_only
        fmt::fmt
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
