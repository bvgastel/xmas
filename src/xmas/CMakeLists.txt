cmake_minimum_required(VERSION 2.6)

project(hello)

# the following line is required only if you don't install FLTK
# or if find_package can't find your installation directory
# it points to the build directory or the base folder,
# not the library folder.
set(FLTK_DIR /path/to/fltk)

find_package(FLTK REQUIRED NO_MODULE)
include(${FLTK_USE_FILE})

add_executable(hello WIN32 hello.cpp)

target_link_libraries(hello fltk)

