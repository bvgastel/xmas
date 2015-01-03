cmake_minimum_required(VERSION 2.8.11)

project(xmd CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets)
find_package(Qt5Svg)

include_directories(include ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Svg_INCLUDE_DIRS})


set(XMAS_SRC_FILES complib.cpp
                   designercontroller.cpp
                   main.cpp
                   mainwindow.cpp
                   modelwindow.cpp
                   network.cpp
                   checker/xmas.cpp)

qt5_add_resources(XMAS_QRC_FILES xmd.qrc)


add_executable(xmd ${XMAS_SRC_FILES} ${XMAS_QRC_FILES})

set_property(TARGET xmd PROPERTY CXX_STANDARD 11)
set_property(TARGET xmd PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(xmd Qt5::Widgets Qt5::Svg)
