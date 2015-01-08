cmake_minimum_required(VERSION 3.1)

project(testmain CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Test)

include_directories(${Qt5Test_INCLUDE_DIRS} ${testlib_SOURCE_DIR} ${xmaslib_SOURCE_DIR} ${xmd_SOURCE_DIR})


set(TESTMAIN_SRC_FILES main.cpp)


add_executable(testmain ${TESTMAIN_SRC_FILES})

set_property(TARGET testmain PROPERTY CXX_STANDARD 11)
set_property(TARGET testmain PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(testmain Qt5::Test testlib)
