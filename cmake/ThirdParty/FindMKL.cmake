# cmake/FindMKL.cmake

# 查找 MKL 的路径
find_path(MKL_INCLUDE_DIR
    NAMES mkl.h
    PATHS
        "C:/Program Files (x86)/Intel/oneAPI/mkl/latest/include"
    DOC "The directory containing MKL headers"
)

find_library(MKL_CORE_LIBRARY
    NAMES mkl_core
    PATHS
        "C:/Program Files (x86)/Intel/oneAPI/mkl/latest/lib/intel64"
    DOC "The MKL core library"
)

find_library(MKL_SEQUENTIAL_LIBRARY
    NAMES mkl_sequential
    PATHS
        "C:/Program Files (x86)/Intel/oneAPI/mkl/latest/lib/intel64"
    DOC "The MKL sequential library"
)

find_library(MKL_INTEL_LP64_LIBRARY
    NAMES mkl_intel_lp64
    PATHS
        "C:/Program Files (x86)/Intel/oneAPI/mkl/latest/lib/intel64"
    DOC "The MKL Intel LP64 library"
)

# 设置 MKL 库
set(MKL_LIBRARIES
    ${MKL_INTEL_LP64_LIBRARY}
    ${MKL_SEQUENTIAL_LIBRARY}
    ${MKL_CORE_LIBRARY}
)

# 检查是否找到所有必需的组件
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MKL DEFAULT_MSG
    MKL_INCLUDE_DIR
    MKL_LIBRARIES
)

# 如果找到 MKL，设置 MKL::MKL 目标
if(MKL_FOUND AND NOT TARGET MKL::MKL)
    add_library(MKL::MKL INTERFACE IMPORTED)
    set_target_properties(MKL::MKL PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${MKL_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${MKL_LIBRARIES}"
    )
endif()
