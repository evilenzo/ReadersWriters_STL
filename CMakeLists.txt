cmake_minimum_required(VERSION 3.21)

project(ReadersWriters_STL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ReadersWriters_STL main.cpp)
target_link_libraries(ReadersWriters_STL pthread)