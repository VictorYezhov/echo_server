cmake_minimum_required(VERSION 3.9)
project(server_boost)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS program_options filesystem thread  REQUIRED)
include_directories( ${Boost_INCLUDE_DIRS} )




add_executable(server_boost main.cpp ClientSession.cpp ClientSession.h Server.cpp Server.h)
target_link_libraries(server_boost ${Boost_LIBRARIES})

