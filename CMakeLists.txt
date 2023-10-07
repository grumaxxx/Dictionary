cmake_minimum_required(VERSION 3.14)
project(Dictionary)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(third_party/googletest)
add_subdirectory(src)

add_executable(freq main.cpp)
target_include_directories(freq PRIVATE src/include)
target_link_libraries(freq dictionary)
