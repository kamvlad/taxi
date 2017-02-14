cmake_minimum_required(VERSION 3.1)

include_directories(..)

add_executable(jsonTests jsonTests.cpp)
target_link_libraries(jsonTests gtest gtest_main pthread)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MONGOCXX REQUIRED libmongocxx)

include_directories(${MONGOCXX_INCLUDE_DIRS})

add_executable(mongodbTests mongodbTests.cpp)
target_link_libraries(mongodbTests ordersRepositoryMongoDB gtest gtest_main pthread)


#add_definitions("-fsanitize=thread")
#add_executable(concTests concTest.cpp)
#target_link_libraries(concTests tsan gtest gtest_main pthread)