set(LIBRARY_NAME "eurora_core")

file(GLOB_RECURSE core_files
    ${CMAKE_SOURCE_DIR}/src/core/*
)

source_group("Core" FILES ${core_files})

find_package(Eigen3 REQUIRED)

add_library(${LIBRARY_NAME} ${core_files})
add_library(eurora::eurora_core ALIAS ${LIBRARY_NAME})

# add project options
target_link_libraries(${LIBRARY_NAME}
    PUBLIC
        ProjectOptions
        Eigen3::Eigen
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
