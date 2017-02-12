cmake_minimum_required(VERSION 2.7)

include_directories(..)

add_executable(jsonTests jsonTests.cpp)
target_link_libraries(jsonTests gtest gtest_main pthread)