cmake_minimum_required(VERSION 3.1)

include_directories(${MONGOCXX_INCLUDE_DIRS})

add_library(ordersRepositoryMongoDB mongodb/ordersRepositoryMongoDB.cpp
                                    mongodb/ordersSimpleRequests.cpp)
target_link_libraries(ordersRepositoryMongoDB ${MONGOCXX_LIBRARIES})

add_library(mongodbComponent SHARED mongodb/mongodbComponent.cpp
                                    mongodb/ordersRepositoryMongoDB.cpp
                                    mongodb/ordersSimpleRequests.cpp)
target_link_libraries(mongodbComponent ${MONGOCXX_LIBRARIES})