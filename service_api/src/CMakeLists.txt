cmake_minimum_required(VERSION 3.16)
project(dragon2_test)

set(BUILD_POSTGRESQL ON CACHE BOOL "Build with postgresql support")
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(drogon)
add_executable(service
        main.cpp

        controllers/User.cc
        controllers/Record.cc
        fillters/JsonCheck.cc
        fillters/TokenCheck.cc

        models/Records.cc
        models/Sessions.cc
        models/Users.cc

        utils.h)
target_link_libraries(service drogon)