cmake_minimum_required(VERSION 2.6)

project(XmasDesigner)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

include_directories(include ${Qt5Widgets_INCLUDE_DIRS})


set(XMAS_SRC_FILES main.cpp
                   mainwindow.cpp
                   canvaswindow.cpp
)

qt5_add_resources(XMAS_QRC_FILES XmasDesigner.qrc)

add_executable(XmasDesigner ${XMAS_SRC_FILES} ${XMAS_QRC_FILES})

target_link_libraries(XmasDesigner ${Qt5Widgets_LIBRARIES})
