cmake_minimum_required(VERSION 2.6)

project(XMD)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

include_directories(include ${Qt5Widgets_INCLUDE_DIRS})


set(XMAS_SRC_FILES main.cpp
                   mainwindow.cpp
                   modelwindow.cpp
)

qt5_add_resources(XMAS_QRC_FILES XDM.qrc)

add_executable(XMD ${XMAS_SRC_FILES} ${XMAS_QRC_FILES})

target_link_libraries(XMD ${Qt5Widgets_LIBRARIES})
