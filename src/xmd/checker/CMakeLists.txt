project(xMAS-checker)
cmake_minimum_required(VERSION 2.8)

if (NOT MINGW)
    OPTION(BUILD_UNIT_TESTS "Build the unit tests for extensive testing" ON)
    OPTION(CONCURRENCY "Enable concurrency to speed up certain processes" ON)
else()
    OPTION(CONCURRENCY "Disable concurrency to speed up certain processes" OFF)
endif (NOT MINGW)
OPTION(MEMORY_DEBUG "Use slow memory allocator and don't cache any memory allocation, for debug only" OFF)
OPTION(REACHABILITY "Build the reachability analysis" OFF)
OPTION(BUILD_ANALYSIS "Build the analysis tools" ON)
OPTION(BUILD_CONVERT "Build the convertor tool" ON)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    #SET(CMAKE_CXX_COMPILER "/opt/local/bin/g++-mp-4.9")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    SET(CMAKE_CXX_COMPILER "g++49")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	#SET(CMAKE_CXX_COMPILER "clang++")
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pipe -g -Werror")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

#INCLUDE_DIRECTORIES("$ENV{HOME}/bin/z3/include")
#find_library(Z3 z3 $ENV{HOME}/bin/z3/bin)

find_package (Threads)

if (BUILD_ANALYSIS)
    set (SRC_LIST cycle.cpp
	state.cpp
	symbolic.cpp
	symbolic-function.cpp
	symbolic-enum-field.cpp
	#symbolic-range-field.cpp
	symbolic-interval-field.cpp
	symbolic-visitor.cpp
	deadlock.cpp
	#constraints.cpp
	messagespec.cpp
	parse.cpp
	export.cpp
	xmas.cpp
	)
    #aux_source_directory(./lib SRC_LIST)

    if (REACHABILITY)
	set (SRC_LIST ${SRC_LIST}
	    reachability/reachable.cpp
	    reachability/transition.cpp
	    reachability/nusmv.cpp
	    reachability/mcrl2.cpp
	    reachability/util.cpp)
    endif(REACHABILITY)

    set (SRC_LIST ${SRC_LIST}
	lib/memory.cpp
	lib/exception.cpp
	lib/simplestring.cpp
	lib/queue.cpp
	lib/stack.cpp
	lib/shared_object.cpp
	lib/stringparse.cpp
	lib/parser.cpp
	lib/parser_json.cpp
	)
    if (CONCURRENCY)
	set (SRC_LIST ${SRC_LIST}
	lib/dispatch.cpp
	lib/thread.cpp
	)
    endif()
    if (MEMORY_DEBUG)
	set (SRC_LIST ${SRC_LIST} lib/memorypool.debug.cpp)
    else (MEMORY_DEBUG)
	set (SRC_LIST ${SRC_LIST} lib/memorypool.cpp)
    endif ()

    add_executable(${PROJECT_NAME} ${SRC_LIST} main.cpp)
    target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

if (BUILD_UNIT_TESTS)
    set (test test.cpp)
    #FILE(GLOB UNITTESTS lib/*.test.cpp *.test.cpp)
    FILE(GLOB UNITTESTS
	lib/*.test.cpp 
	main.test.cpp)
    if (REACHABILITY)
	FILE(GLOB UNITTESTS-REACHABILITY
	    reachability/*.test.cpp 
	    reachability/test_util.cpp)
	set(UNITTESTS ${UNITTESTS} ${UNITTESTS-REACHABILITY})
    endif()
    add_executable(${PROJECT_NAME}-test ${test} ${UNITTESTS} ${SRC_LIST})
    set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
    add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
    add_dependencies(${PROJECT_NAME}-test googletest)
    INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})
    # need google-test
    target_link_libraries(${PROJECT_NAME}-test ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a)
endif (BUILD_UNIT_TESTS)
endif(BUILD_ANALYSIS)

if (BUILD_CONVERT)
    set (CONVERT_SRC_LIST
	cycle.cpp
	state.cpp
	symbolic.cpp
	symbolic-function.cpp
	symbolic-enum-field.cpp
	symbolic-interval-field.cpp
	symbolic-visitor.cpp
	messagespec.cpp
	parse.cpp
	export.cpp
	xmas.cpp
	)

    set (CONVERT_SRC_LIST ${CONVERT_SRC_LIST}
	lib/memory.cpp
	lib/exception.cpp
	lib/simplestring.cpp
	lib/shared_object.cpp
	lib/stringparse.cpp
	lib/parser.cpp
	lib/parser_json.cpp
	)
    if (MEMORY_DEBUG)
	set (CONVERT_SRC_LIST ${CONVERT_SRC_LIST} lib/memorypool.debug.cpp)
    else (MEMORY_DEBUG)
	set (CONVERT_SRC_LIST ${CONVERT_SRC_LIST} lib/memorypool.cpp)
    endif ()

    add_executable(${PROJECT_NAME}-convert ${CONVERT_SRC_LIST} convert.cpp)
endif(BUILD_CONVERT)


#target_link_libraries(${PROJECT_NAME} ${Z3})

if (MEMORY_DEBUG)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMEMORY_DEBUG")
endif (MEMORY_DEBUG)

# unit test executable

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
ENDIF()


if (MINGW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS} -static-libgcc -s")
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static-libgcc -static-libstdc++ -s")
endif()
