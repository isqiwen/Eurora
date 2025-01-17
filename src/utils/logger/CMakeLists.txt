set(LIBRARY_NAME "logger")

find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

file(GLOB_RECURSE src_files
    ${CMAKE_SOURCE_DIR}/src/utils/logger/logger.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/pattern/singleton.h
    ${CMAKE_SOURCE_DIR}/include/eurora/utils/logger.h
)

source_group("src" FILES ${src_files})

add_library(${LIBRARY_NAME} SHARED ${src_files})
add_library(eurora::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
        ProjectOptions
        eurora::time
        spdlog::spdlog_header_only
        fmt::fmt
        nlohmann_json::nlohmann_json
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
