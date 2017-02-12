cmake_minimum_required(VERSION 3.1)

#TODO Move to up
find_package(PkgConfig REQUIRED)
pkg_check_modules(MONGOCXX REQUIRED libmongocxx)

include_directories(${MONGOCXX_INCLUDE_DIRS})

add_library(ordersRepositoryMongoDB OrdersRepositoryMongoDB.cpp)
target_link_libraries(ordersRepositoryMongoDB ${MONGOCXX_LIBRARIES})