file(GLOB_RECURSE test_sources ${CMAKE_SOURCE_DIR}/test/*.cpp)

foreach(file ${test_sources})
  string(REGEX REPLACE "(.*/)([a-zA-Z0-9_ ]+)(\.cpp)" "\\2" test_name ${file})
  add_executable(${test_name}_ut ${file})

  target_compile_features(${test_name}_ut PUBLIC cxx_std_17)

  if (${CMAKE_PROJECT_NAME}_ENABLE_CODE_COVERAGE)
    target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
    target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
    message(STATUS "Code coverage is enabled for GCC.")
  endif()

  if(${CMAKE_PROJECT_NAME}_USE_GTEST)
    find_package(GTest REQUIRED)
    target_link_libraries(
      ${test_name}_ut
      PUBLIC
        Eurora::common
        GTest::GTest
        GTest::Main
    )
  elseif(${CMAKE_PROJECT_NAME}_USE_CATCH2)
    find_package(Catch2 REQUIRED)
    target_link_libraries(
      ${test_name}_ut
      PUBLIC
        Common
        Catch2::Catch2
    )
  else()
    message(FATAL_ERROR "No valid unit test framework configured.")
  endif()

  get_filename_component(test_dir ${file} DIRECTORY)
  string(REPLACE ${CMAKE_SOURCE_DIR} "" test_dir_group ${test_dir})
  source_group("${test_dir_group}" FILES ${file})

  add_test(
    NAME
      ${test_name}
    COMMAND
      ${test_name}_ut
  )
endforeach()
