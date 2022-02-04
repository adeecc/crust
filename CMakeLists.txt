cmake_minimum_required(VERSION 3.13.0)

# Define the Project Name and Description
project(
    CRust 
    VERSION 0.1.0
    LANGUAGES CXX 
    DESCRIPTION "A Compiler Frontend for the CRust Language :)")


# Do it at some point
# enable_testing() 

# Set required C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")

# Let's nicely support folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src)
# add_subdirectory(tests)
