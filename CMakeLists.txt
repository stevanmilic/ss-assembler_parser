cmake_minimum_required (VERSION 2.8)
project (AssemblerParser)

# CXX="g++-4.6" C="gcc-4.6" cmake -DCMAKE_BUILD_TYPE=Debug <path>

SET(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++0x")
SET(CMAKE_C_FLAGS "-std=c99 -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes")
 
include_directories(assembler_parser/include/ assembler_linker/include assembler_emulator/include regex/include/ exprtk/ elf_format/include)

link_directories(/home/stevan/etf/sistSoft/projekat/regex/lib)

file(GLOB PARSER_SOURCES "assembler_parser/src/*.cpp" "elf_format/src/elffile.cpp" "elf_format/src/objectelffile.cpp" "elf_format/src/writeobjectelffile.cpp")
file(GLOB LINKER_SOURCES "assembler_linker/src/*.cpp" "assembler_parser/src/linemanipulation.cpp" "assembler_parser/src/parser.cpp" "assembler_parser/src/elftablebuilder.cpp")
file(GLOB ELF_FORMAT_SOURCES "elf_format/src/*.cpp")
file(GLOB EMULATOR_SOURCES "assembler_emulator/src/*.cpp" "assembler_emulator/src/*.c")

add_executable(asm_parser ${PARSER_SOURCES})
# add_executable(asm_linker ${LINKER_SOURCES} ${ELF_FORMAT_SOURCES})
add_executable(asm_emulator ${EMULATOR_SOURCES} ${LINKER_SOURCES} ${ELF_FORMAT_SOURCES})

TARGET_LINK_LIBRARIES(asm_parser libboost_regex.so)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
