# Create anagram library

set(LIBRARY_NAME "Common")

set(SRC_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/exception/Exception.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/exception/ErrorCodes.cpp
)

add_library(${LIBRARY_NAME} ${SRC_SOURCE})

# Add include directories for building and installing anagram
target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# add project options
target_link_libraries(${LIBRARY_NAME} PRIVATE ProjectOptions)

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
