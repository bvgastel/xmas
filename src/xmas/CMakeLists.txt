cmake_minimum_required(VERSION 3.1)

project(xmaslib CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)

include_directories(${Qt5Core_INCLUDE_DIRS})


set(SRC_FILES component.cpp
              noc.cpp
              channel.cpp
              composite.cpp
              primitive.cpp)
                   


add_library(xmaslib ${SRC_FILES})

set_property(TARGET xmaslib PROPERTY CXX_STANDARD 11)
set_property(TARGET xmaslib PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(xmaslib Qt5::Core)
