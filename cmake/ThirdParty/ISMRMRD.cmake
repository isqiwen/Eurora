include(FetchContent)

FetchContent_Declare(
    ismrmrd
    GIT_REPOSITORY https://github.com/ismrmrd/ismrmrd.git
    GIT_TAG v1.14.2
)

FetchContent_MakeAvailable(ismrmrd)
