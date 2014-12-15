cmake_minimum_required(VERSION 2.6)

project(xmas)

# the following line is required only if you don't install FLTK
# or if find_package can't find your installation directory
# it points to the build directory or the base folder,
# not the library folder.
#set(FLTK_DIR /path/to/fltk)

find_package(FLTK REQUIRED)
include_directories(include ${FLTK_INCLUDE_SEARCH_PATH})

set(XMAS_SRC_FILES main.cpp
                   ui_window_main.cpp
)

add_executable(xmas ${XMAS_SRC_FILES})

target_link_libraries(xmas ${FLTK_LIBRARIES})
