cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")

find_package(PkgConfig REQUIRED)
pkg_check_modules(MONGOCXX REQUIRED libmongocxx)

include_directories(${MONGOCXX_INCLUDE_DIRS})

add_library(ordersService SHARED services/orders.cpp data/JSONPresentation.cpp)
target_link_libraries(ordersService ${MONGOCXX_LIBRARIES})

set (COMPONENTS
        libordersService.so
        db/libmongodbComponent.so)

add_custom_target(deploy_OrdersService ${CMAKE_SOURCE_DIR}/deploy.sh ${CMAKE_SOURCE_DIR}/configs/basic.xml ${COMPONENTS}
        DEPENDS ordersService mongodbComponent)

include_directories(.)

add_subdirectory(data)
add_subdirectory(db)
add_subdirectory(tests)
