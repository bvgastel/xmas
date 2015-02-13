#!/bin/sh
g++ --std=c++11 -w -o xmas-convert lib/memorypool.cpp lib/memory.cpp lib/exception.cpp lib/simplestring.cpp lib/shared_object.cpp lib/stringparse.cpp lib/parser.cpp lib/parser_json.cpp cycle.cpp state.cpp symbolic.cpp symbolic-function.cpp symbolic-enum-field.cpp symbolic-interval-field.cpp symbolic-visitor.cpp messagespec.cpp parse.cpp export.cpp xmas.cpp convert.cpp
