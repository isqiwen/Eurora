# Create test executable
add_executable(common_test anagram_test.cpp)

# Link test executable to dependent libraries
target_link_libraries(common_test
    PRIVATE
        GTest::gtest_main
        ProjectOptions
        Anagram
)

# Enable test discovery
gtest_discover_tests(common_test)
