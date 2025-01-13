set(TEST_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/TestBuild")
file(MAKE_DIRECTORY ${TEST_BUILD_DIR})

file(WRITE "${TEST_BUILD_DIR}/CMakeLists.txt" "
cmake_minimum_required(VERSION 3.15)
project(TestLibraryModule LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH \"${CMAKE_SOURCE_DIR}/cmake/functions\")

include(AddLibraryModule)

add_library_module(TestLibrary TESTLIB TestNamespace
    ${CMAKE_SOURCE_DIR}/src/module_a/module_a.cpp
)
")

# 配置并构建
execute_process(COMMAND ${CMAKE_COMMAND} -S "${TEST_BUILD_DIR}" -B "${TEST_BUILD_DIR}/build")
execute_process(COMMAND ${CMAKE_COMMAND} --build "${TEST_BUILD_DIR}/build")

# 验证安装的结果
execute_process(COMMAND bash "${CMAKE_SOURCE_DIR}/tests/test_scripts/verify_install.sh")
