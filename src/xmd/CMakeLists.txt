cmake_minimum_required(VERSION 2.8.11)

project(XMD CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(Qt5Widgets)

include_directories(include ${Qt5Widgets_INCLUDE_DIRS})


set(XMAS_SRC_FILES main.cpp
                   mainwindow.cpp
                   modelwindow.cpp)

qt5_add_resources(XMAS_QRC_FILES XMD.qrc)


add_executable(XMD ${XMAS_SRC_FILES} ${XMAS_QRC_FILES})

set_property(TARGET XMD PROPERTY CXX_STANDARD 11)
set_property(TARGET XMD PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(XMD Qt5::Widgets)
