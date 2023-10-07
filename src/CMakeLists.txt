cmake_minimum_required(VERSION 3.6)
project(dictionary C CXX)

add_library(dictionary EXCLUDE_FROM_ALL
        file_manager.cpp
        dictionary.cpp)
target_include_directories(dictionary PUBLIC include)

enable_testing()
add_executable(dictionary_test EXCLUDE_FROM_ALL test/dictionary_test.cpp)

target_link_libraries(dictionary_test dictionary gtest gtest_main)
target_include_directories(dictionary_test PRIVATE ../include)
include(GoogleTest)
gtest_discover_tests(dictionary_test)

file(COPY test/testfiles/in.txt test/testfiles/expected_out.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/testfiles)
