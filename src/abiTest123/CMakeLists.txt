cmake_minimum_required(VERSION 2.8)

project(abiTest123)

find_package(FLTK REQUIRED)

add_executable(abiTest123 main.cpp)

target_link_libraries(abiTest123 fltk)
