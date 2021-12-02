# Author:
#   Brymer Meneses and Aaron Paul Gapud

# NOTE:
#   This file contains the build process of our game.
#   We did this using cmake since it automates the installation of
#   the external libraries used in this game.

# Set the minimum cmake version. This ensures that
# cmake will be able to run certain features which many 
# not be available to older versions of CMake.
cmake_minimum_required(VERSION 3.21)

# Set the name of the project.
project(ghostless-pacman)

# Store the compiled folder in the bin directory.
# The bin directory will also contain the DLL files which are 
# necessary for running the game, since it depends on external gui 
# libraries.
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin )

# Compile all the C files and headers located in the src/ folder
file(GLOB ALL_SRCS
    "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/src/*.h"
    )
add_executable(${PROJECT_NAME} ${ALL_SRCS})

# Handle the library dependencies the libraries. This ensures that 
# the external libraries we used: SDL and SDL_image will be accessed by 
# the game.
include(cmake/handle_dependencies.cmake)

