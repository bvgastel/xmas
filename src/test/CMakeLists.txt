cmake_minimum_required(VERSION 3.1)

project(testlib CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Test)

include_directories(${Qt5Test_INCLUDE_DIRS} ${xmaslib_SOURCE_DIR} ${xmd_SOURCE_DIR})


set(TEST_SRC_FILES testnoc.cpp
                   testcomponent.cpp
                   testnetwork.cpp
                   testrunner.cpp)

add_library(testlib ${TEST_SRC_FILES})

set_property(TARGET testlib PROPERTY CXX_STANDARD 11)
set_property(TARGET testlib PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(testlib Qt5::Test xmaslib xmd)
