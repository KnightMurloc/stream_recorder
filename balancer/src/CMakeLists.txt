cmake_minimum_required(VERSION 3.16)
project(balancer)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(drogon)
add_subdirectory(concurrentqueue)

add_executable(service main.cpp queue.cc)
target_link_libraries(service drogon)