cmake_minimum_required(VERSION 3.14)
project(dictionary C CXX)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_library(dictionary EXCLUDE_FROM_ALL
        writer.cpp
        reader.cpp
        dictionary.cpp
        include/timer.h
)
target_include_directories(dictionary PUBLIC include)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/run_python_script.stamp
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/src/test/generate_test_files.py
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/run_python_script.stamp
        DEPENDS ${CMAKE_SOURCE_DIR}/src/test/generate_test_files.py
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testfiles)

add_custom_target(
        prepare_test_files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/test/testfiles ${CMAKE_CURRENT_BINARY_DIR}/testfiles
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/run_python_script.stamp
)

enable_testing()
add_executable(dictionary_test EXCLUDE_FROM_ALL test/dictionary_test.cpp)

add_dependencies(dictionary_test prepare_test_files)

target_link_libraries(dictionary_test dictionary gtest gtest_main)
target_include_directories(dictionary_test PRIVATE ../include)
include(GoogleTest)
gtest_discover_tests(dictionary_test)
