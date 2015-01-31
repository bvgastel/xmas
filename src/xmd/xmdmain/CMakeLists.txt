cmake_minimum_required(VERSION 3.1)

project(xmdmain CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${xmd_SOURCE_DIR})


set(XMDMAIN_SRC_FILES main.cpp)


add_executable(xmdmain ${XMDMAIN_SRC_FILES})

set_property(TARGET xmdmain PROPERTY CXX_STANDARD 11)
set_property(TARGET xmdmain PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(xmdmain Qt5::Widgets xmd)
