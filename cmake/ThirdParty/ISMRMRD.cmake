include(FetchContent)

FetchContent_Declare(
    ismrmrd
    GIT_REPOSITORY https://github.com/isqiwen/ismrmrd.git
    GIT_TAG master
    CMAKE_ARGS
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_BINARY_DIR}/generators/conan_toolchain.cmake
        -DBUILD_TESTS=OFF
        -DBUILD_UTILITIES=OFF
        -DBUILD_EXAMPLES=OFF
)

FetchContent_MakeAvailable(ismrmrd)
