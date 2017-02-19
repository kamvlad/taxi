cmake_minimum_required(VERSION 3.1)

include_directories(..)

add_executable(jsonTests jsonTests.cpp)
target_link_libraries(jsonTests jsonresentation gtest gtest_main pthread)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MONGOCXX REQUIRED libmongocxx)

include_directories(${MONGOCXX_INCLUDE_DIRS})

add_executable(mongodbTests mongodb/OrdersSimpleRequestsTests.cpp mongodb/OrdersRepositoryTests.cpp)
target_link_libraries(mongodbTests ordersRepositoryMongoDB gtest gtest_main pthread)

add_executable(OrdersRepositoryConcurrentTests mongodb/OrdersRepositoryConcurrentTests.cpp)
target_link_libraries(OrdersRepositoryConcurrentTests tsan ordersRepositoryMongoDB gtest gtest_main pthread)
target_compile_options(OrdersRepositoryConcurrentTests PUBLIC "-fsanitize=thread")
