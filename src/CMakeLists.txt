cmake_minimum_required(VERSION 3.13.0)

file (GLOB LEXER_SOURCES         "lexer/*.cpp" "lexer/*.hpp" "lexer/*.h")
file (GLOB PARSER_SOURCES        "parser/*.cpp" "parser/*.hpp" "parser/*.h")
file (GLOB UTILS_SOURCES        "utils/*.cpp" "utils/*.hpp" "utils/*.h")

source_group(lexer\\LEXER FILES ${LEXER_SOURCES})
source_group(parser\\PARSER FILES ${PARSER_SOURCES})
source_group(utils\\UTILS FILES ${UTILS_SOURCES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/bin)

add_library(lexer STATIC ${LEXER_SOURCES})
add_library(parser STATIC ${PARSER_SOURCES})
add_library(utils STATIC ${UTILS_SOURCES})

add_executable (crusty_compiler main.cpp)
target_link_libraries(crusty_compiler lexer parser utils)


