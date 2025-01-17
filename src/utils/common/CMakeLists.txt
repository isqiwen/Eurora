set(LIBRARY_NAME "common")

file(GLOB_RECURSE common_files
    ${CMAKE_SOURCE_DIR}/src/utils/common/*.h
    ${CMAKE_SOURCE_DIR}/src/utils/common/*.hpp
    ${CMAKE_SOURCE_DIR}/src/utils/common/*.cpp
)

file(GLOB_RECURSE exception_files
    ${CMAKE_SOURCE_DIR}/include/eurora/utils/error_codes.h
    ${CMAKE_SOURCE_DIR}/include/eurora/utils/exception.h
)

source_group("Exception" FILES ${exception_files})
source_group("Common" FILES ${common_files})

find_package(Boost REQUIRED)

add_library(${LIBRARY_NAME} SHARED ${exception_files} ${common_files})
add_library(eurora::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

target_link_libraries(${LIBRARY_NAME}
    PRIVATE
        ProjectOptions
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
