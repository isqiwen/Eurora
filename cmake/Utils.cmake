#
# Print a message only if the `VERBOSE_OUTPUT` option is on
#

function(verbose_message content)
  if(${PROJECT_NAME}_VERBOSE_OUTPUT)
    message(STATUS ${content})
  endif()
endfunction()

#
# Add a target for formating the project using `clang-format` (i.e: cmake --build build --target clang-format)
#

function(add_clang_format_target)
  if(NOT ${PROJECT_NAME}_CLANG_FORMAT_BINARY)
    find_program(${PROJECT_NAME}_CLANG_FORMAT_BINARY clang-format)
  endif()

  if(${PROJECT_NAME}_CLANG_FORMAT_BINARY)
    if(${PROJECT_NAME}_BUILD_EXECUTABLE)
      add_custom_target(clang-format
        COMMAND ${${PROJECT_NAME}_CLANG_FORMAT_BINARY}
        -i ${exe_sources} ${headers}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
    elseif(${PROJECT_NAME}_BUILD_HEADERS_ONLY)
      add_custom_target(clang-format
        COMMAND ${${PROJECT_NAME}_CLANG_FORMAT_BINARY}
        -i ${headers}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
    else()
      add_custom_target(clang-format
        COMMAND ${${PROJECT_NAME}_CLANG_FORMAT_BINARY}
        -i ${sources} ${headers}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
    endif()

    message(STATUS "Format the project using the `clang-format` target (i.e: cmake --build build --target clang-format).\n")
  endif()
endfunction()

#
# Function to print detailed project configuration and paths
#
function(print_project_configuration)
  message(STATUS "################################################################################")
  message(STATUS "#                            Project Configuration                             ")
  message(STATUS "################################################################################")

  # CMake version and generator
  message(STATUS "CMake version: ${CMAKE_VERSION}")
  message(STATUS "CMake generator: ${CMAKE_GENERATOR}")
  if (CMAKE_GENERATOR_PLATFORM)
    message(STATUS "CMake generator platform: ${CMAKE_GENERATOR_PLATFORM}")
  endif()
  if (CMAKE_GENERATOR_TOOLSET)
    message(STATUS "CMake generator toolset: ${CMAKE_GENERATOR_TOOLSET}")
  endif()

  # Project paths
  message(STATUS "Project source directory (CMAKE_SOURCE_DIR): ${CMAKE_SOURCE_DIR}")
  message(STATUS "Project binary directory (CMAKE_BINARY_DIR): ${CMAKE_BINARY_DIR}")
  message(STATUS "Install prefix (CMAKE_INSTALL_PREFIX): ${CMAKE_INSTALL_PREFIX}")
  message(STATUS "CMake module path (CMAKE_MODULE_PATH): ${CMAKE_MODULE_PATH}")

  # Build directories
  message(STATUS "CMake current binary directory (CMAKE_CURRENT_BINARY_DIR): ${CMAKE_CURRENT_BINARY_DIR}")
  message(STATUS "CMake current source directory (CMAKE_CURRENT_SOURCE_DIR): ${CMAKE_CURRENT_SOURCE_DIR}")
  if (CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    message(STATUS "Runtime output directory (CMAKE_RUNTIME_OUTPUT_DIRECTORY): ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  endif()
  if (CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    message(STATUS "Library output directory (CMAKE_LIBRARY_OUTPUT_DIRECTORY): ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
  endif()
  if (CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    message(STATUS "Archive output directory (CMAKE_ARCHIVE_OUTPUT_DIRECTORY): ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
  endif()

  # Build type
  if (CMAKE_BUILD_TYPE)
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
  else()
    message(STATUS "Build type: Not specified (multi-configuration generator)")
  endif()

  # Compiler information
  message(STATUS "C++ compiler (CMAKE_CXX_COMPILER): ${CMAKE_CXX_COMPILER}")
  message(STATUS "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
  message(STATUS "C++ compiler flags (CMAKE_CXX_FLAGS): ${CMAKE_CXX_FLAGS}")
  message(STATUS "C++ compiler flags (Debug): ${CMAKE_CXX_FLAGS_DEBUG}")
  message(STATUS "C++ compiler flags (Release): ${CMAKE_CXX_FLAGS_RELEASE}")
  message(STATUS "C++ standard (CMAKE_CXX_STANDARD): ${CMAKE_CXX_STANDARD}")

  # Install directories
  message(STATUS "Install binary directory (CMAKE_INSTALL_BINDIR): ${CMAKE_INSTALL_BINDIR}")
  message(STATUS "Install library directory (CMAKE_INSTALL_LIBDIR): ${CMAKE_INSTALL_LIBDIR}")
  message(STATUS "Install include directory (CMAKE_INSTALL_INCLUDEDIR): ${CMAKE_INSTALL_INCLUDEDIR}")
  message(STATUS "Install CMake package directory: ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}")

  # Additional directories
  message(STATUS "CMake cache directory (CMAKE_CACHEFILE_DIR): ${CMAKE_CACHEFILE_DIR}")
  if (CMAKE_TEMP_DIRECTORY)
    message(STATUS "Temporary directory (CMAKE_TEMP_DIRECTORY): ${CMAKE_TEMP_DIRECTORY}")
  endif()

  # Additional custom project variables (example)
  if (DEFINED PROJECT_CUSTOM_PATH)
    message(STATUS "Custom project path: ${PROJECT_CUSTOM_PATH}")
  endif()

  message(STATUS "################################################################################")
endfunction()
