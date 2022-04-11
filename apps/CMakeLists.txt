cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/bin")

add_executable(
    app 
    src/app.cpp
)

target_compile_features(app PRIVATE cxx_std_20)

target_link_libraries(app PRIVATE crusty_compiler)