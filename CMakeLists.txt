cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")

add_library(sleepService SHARED benchmarks/sleepService.cpp)

add_custom_target(deploySleepService ${CMAKE_SOURCE_DIR}/deploy.sh ${CMAKE_SOURCE_DIR}/configs/basic.xml libsleepService.so
                    DEPENDS sleepService)

add_library(ordersService SHARED services/orders.cpp)

add_custom_target(deploy_OrdersService ${CMAKE_SOURCE_DIR}/deploy.sh ${CMAKE_SOURCE_DIR}/configs/basic.xml libordersService.so
        DEPENDS ordersService)

include_directories(.)

add_subdirectory(db)
add_subdirectory(tests)
