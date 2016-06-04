cmake_minimum_required (VERSION 2.8)
project (AssemblerParser)

# CXX="g++-4.6" cmake -DCMAKE_BUILD_TYPE=Debug <path>

SET(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++0x")
 
include_directories(assembler_parser/include/ regex/include/ exprtk/)

link_directories(/home/stevan/etf/sistSoft/projekat/regex/lib)

file(GLOB SOURCES "assembler_parser/src/*.cpp")

# add_executable(asm_parser assembler_parser/src/main.cpp)
add_executable(asm_parser ${SOURCES})

TARGET_LINK_LIBRARIES(asm_parser libboost_regex.so)
