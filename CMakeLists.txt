cmake_minimum_required(VERSION 2.7)

set(CMAKE_CXX_FLAGS "-std=c++11")

add_library(sleepService SHARED benchmarks/sleepService.cpp)

add_custom_target(deploySleepService ${CMAKE_SOURCE_DIR}/deploy.sh ${CMAKE_SOURCE_DIR}/configs/basic.xml libsleepService.so
                    DEPENDS sleepService)