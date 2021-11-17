cmake_minimum_required(VERSION 3.21)

project(ghostless-pacman, VERSION 0.1)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR} )

set (SDL_VERSION "2.0.16")
set (SDL_IMAGE_VERSION "2.0.5")

set (SDL_SOURCE_DIR "${PROJECT_SOURCE_DIR}/build/SDL2")
set (SDL_INSTALL_DIR "${PROJECT_SOURCE_DIR}/external/SDL2")

set (SDL_IMAGE_SOURCE_DIR "${PROJECT_SOURCE_DIR}/build/SDL2_image")
set (SDL_IMAGE_INSTALL_DIR "${PROJECT_SOURCE_DIR}/external/SDL2_image")

include(ExternalProject)

ExternalProject_Add(
    SDL2
    URL "https://github.com/libsdl-org/SDL/archive/refs/tags/release-${SDL_VERSION}.zip"
    SOURCE_DIR "${SDL_SOURCE_DIR}"
    BUILD_IN_SOURCE TRUE
    CONFIGURE_COMMAND ${SDL_SOURCE_DIR}/configure --prefix=${SDL_INSTALL_DIR}
    )

ExternalProject_Add(
    SDL2_image
    URL "https://github.com/libsdl-org/SDL_image/archive/refs/tags/release-${SDL_IMAGE_VERSION}.zip"
    SOURCE_DIR "${SDL_IMAGE_SOURCE_DIR}"
    BUILD_IN_SOURCE TRUE
    CONFIGURE_COMMAND ${SDL_IMAGE_SOURCE_DIR}/configure --prefix=${SDL_IMAGE_INSTALL_DIR}
    )

set (SDL_LIBRARIES "${SDL_INSTALL_DIR}/lib")
set (SDL_INCLUDE_DIRS "${SDL_INSTALL_DIR}/include/SDL2/")

set (SDL_IMAGE_LIBRARIES "${SDL_IMAGE_INSTALL_DIR}/lib")
set (SDL_IMAGE_INCLUDE_DIRS "${SDL_IMAGE_INSTALL_DIR}/include/SDL2")

include_directories(
    "${SDL_INCLUDE_DIRS}"
    "${SDL_IMAGE_INCLUDE_DIRS}"
    )

file(GLOB ALL_SRCS
    "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/src/*.h"
    )

find_package (Threads)

if (WIN32) 
    set(lib dll)
endif ()

if (UNIX)
    set(lib a)
endif ()

add_executable(ghostless-pacman ${ALL_SRCS})

add_dependencies(ghostless-pacman SDL2)
add_dependencies(ghostless-pacman SDL2_image)

# Link SDL Dependencies
target_link_libraries(ghostless-pacman "${SDL_LIBRARIES}/libSDL2main.${lib}")
target_link_libraries(ghostless-pacman "${SDL_LIBRARIES}/libSDL2.${lib}")
target_link_libraries(ghostless-pacman "${SDL_IMAGE_LIBRARIES}/libSDL2_image.${lib}")

# Link C libraries 
target_link_libraries(ghostless-pacman ${CMAKE_DL_LIBS})
target_link_libraries(ghostless-pacman ${CMAKE_THREAD_LIBS_INIT})

# Link Math library
target_link_libraries(ghostless-pacman m)
