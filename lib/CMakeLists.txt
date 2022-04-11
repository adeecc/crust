cmake_minimum_required(VERSION 3.16.0)

add_library(
    crusty_compiler
    STATIC
    src/parser/lexer.cpp
    src/parser/parser.cpp
    src/common/errorlogger.cpp
    src/common/sourceloc.cpp
)

# All users of this library will need at least C++20
target_compile_features(crusty_compiler PUBLIC cxx_std_20)

# We need this directory, and users of our library will need it too
target_include_directories(
    crusty_compiler
    PUBLIC
    include/
)