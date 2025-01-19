set(LIBRARY_NAME "time")

file(GLOB_RECURSE src_files
    ${CMAKE_SOURCE_DIR}/src/export_macros.h
    ${CMAKE_SOURCE_DIR}/src/utils/time/datetime_provider.h
    ${CMAKE_SOURCE_DIR}/src/utils/time/datetime_provider.cpp
)

source_group("src" FILES ${src_files})

add_library(${LIBRARY_NAME} SHARED ${src_files})
add_library(eurora::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
        ProjectOptions
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
