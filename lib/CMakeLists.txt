cmake_minimum_required(VERSION 3.16.0)

file (GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${CRust_SOURCE_DIR}/include/crust/*.hpp")
file (GLOB_RECURSE SOURCES_LIST CONFIGURE_DEPENDS "${CRust_SOURCE_DIR}/lib/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/bin/lib")

# Make an automatic library - will be static or dynamic based on user setting
add_library(crusty_compiler ${SOURCES_LIST} ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(crusty_compiler PUBLIC "../include")

# All users of this library will need at least C++17
target_compile_features(crusty_compiler PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
source_group(
    TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST}
)