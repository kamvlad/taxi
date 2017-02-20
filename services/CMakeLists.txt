cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")

include_directories(${MONGOCXX_INCLUDE_DIRS})

set (SOURCES
            ../i18n/Messages.cpp
            ../data/jsonPresentation.cpp
            orders.cpp)

add_library(ordersService SHARED ${SOURCES})
target_link_libraries(ordersService ${MONGOCXX_LIBRARIES})
