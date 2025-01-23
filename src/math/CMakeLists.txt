set(LIBRARY_NAME "math")

find_package(Eigen3 REQUIRED)
find_package(NumCpp REQUIRED)
find_package(Armadillo REQUIRED)

file(GLOB_RECURSE src_files
    ${CMAKE_SOURCE_DIR}/src/math/backends/armadillo/vector_armadillo.h
    ${CMAKE_SOURCE_DIR}/src/math/backends/armadillo/vector_armadillo.cpp
    ${CMAKE_SOURCE_DIR}/src/math/backends/eigen/vector_eigen.h
    ${CMAKE_SOURCE_DIR}/src/math/backends/eigen/vector_eigen.cpp
    ${CMAKE_SOURCE_DIR}/src/math/backends/numcpp/vector_numcpp.h
    ${CMAKE_SOURCE_DIR}/src/math/backends/numcpp/vector_numcpp.cpp
    ${CMAKE_SOURCE_DIR}/include/eurora/math/types.h
    ${CMAKE_SOURCE_DIR}/include/eurora/math/backend_type.h
    ${CMAKE_SOURCE_DIR}/include/eurora/math/vector_ops.hpp
    ${CMAKE_SOURCE_DIR}/include/eurora/math/math.hpp
    ${CMAKE_SOURCE_DIR}/include/eurora/utils/enum_utils.hpp
    ${CMAKE_SOURCE_DIR}/include/eurora/utils/export_macros.h
)

source_group("src" FILES ${src_files})

add_library(${LIBRARY_NAME} SHARED ${src_files})
add_library(eurora::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
        ProjectOptions
        NumCpp::NumCpp
        Eigen3::Eigen
        armadillo::armadillo
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
