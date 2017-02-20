cmake_minimum_required(VERSION 3.1)

add_library(jsonresentation jsonPresentation.h jsonPresentation.cpp)
target_link_libraries(jsonresentation ${MONGOCXX_LIBRARIES})