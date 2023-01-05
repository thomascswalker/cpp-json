cmake_minimum_required(VERSION 3.24)
project(cpp_json)

include_directories(src)

set(PROJECT_HEADERS src/json.h)
set(PROJECT_SOURCES main.cpp src/json.cpp)
set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_json main.cpp src/json.h src/json.cpp)
