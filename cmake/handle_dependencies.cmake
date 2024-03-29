
include(FetchContent)

# Handle Dependencies for MacOS
if (APPLE) 

  # Add the include directory for SDL2
  target_include_directories(${PROJECT_NAME} PRIVATE /opt/homebrew/include)

  # Define the _THREAD_SAFE macro
  target_compile_definitions(${PROJECT_NAME} PRIVATE _THREAD_SAFE)

  # Link with the SDL2 library
  target_link_directories(${PROJECT_NAME} PRIVATE /opt/homebrew/lib)
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2_image SDL2_mixer)
endif ()

# Handle Dependencies for Linux
if (LINUX)
    FetchContent_Declare(
        SDL2_CMAKE 
        GIT_REPOSITORY  https://github.com/aminosbh/sdl2-cmake-modules
        GIT_TAG         origin/master
        GIT_PROGRESS    true
        SOURCE_DIR      ${PROJECT_SOURCE_DIR}/external/cmake
    )
    
    FetchContent_MakeAvailable(SDL2_CMAKE)

    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/external/cmake)

    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)

    include_directories(${PROJECT_NAME} 
        ${SDL2_INCLUDE_DIRS} 
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
    )
    target_link_libraries(${PROJECT_NAME} 
        ${SDL2_LIBRARIES} 
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
    )
endif ()

## Handle Dependencies for Windows
if (WIN32)
    if (MSVC)
        message( SEND_ERROR "This project is configured to use MinGW" )
    endif ()

    add_definitions(-DSDL_MAIN_HANDLED)


    FetchContent_Declare(
        SDL2
        URL             https://www.libsdl.org/release/SDL2-devel-2.0.16-mingw.tar.gz        
        SOURCE_DIR      ${PROJECT_SOURCE_DIR}/external/SDL2
    )

    FetchContent_Declare(
        SDL2_image
        URL             https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-mingw.tar.gz
        SOURCE_DIR      ${PROJECT_SOURCE_DIR}/external/SDL2_image
    )

    FetchContent_Declare(
        SDL2_mixer
        URL             https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.4-mingw.tar.gz
        SOURCE_DIR      ${PROJECT_SOURCE_DIR}/external/SDL2_mixer
    )
    
    message("Downloading dependencies...")
    FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_mixer)

    message("Downloading complete...")
    set(ARCH x86_64-w64-mingw32)

    include_directories(
        ${PROJECT_NAME}
        ${PROJECT_SOURCE_DIR}/external/SDL2_image/${ARCH}/include/
        ${PROJECT_SOURCE_DIR}/external/SDL2_mixer/${ARCH}/include/
        ${PROJECT_SOURCE_DIR}/external/SDL2/${ARCH}/include/
        ${PROJECT_SOURCE_DIR}/external/SDL2/${ARCH}/include/SDL2
        )
    target_link_libraries(
        ${PROJECT_NAME}
        ${PROJECT_SOURCE_DIR}/external/SDL2/${ARCH}/lib/libSDL2.dll.a
        ${PROJECT_SOURCE_DIR}/external/SDL2/${ARCH}/lib/libSDL2main.a
        ${PROJECT_SOURCE_DIR}/external/SDL2_image/${ARCH}/lib/libSDL2_image.dll.a
        ${PROJECT_SOURCE_DIR}/external/SDL2_mixer/${ARCH}/lib/libSDL2_mixer.dll.a
    )


    file(GLOB 
        SDL2_IMAGE_DLLS
        ${PROJECT_SOURCE_DIR}/external/SDL2_image/${ARCH}/bin/*.dll
        )

    file(GLOB
        SDL2_MIXER_DLLS
        ${PROJECT_SOURCE_DIR}/external/SDL2_mixer/${ARCH}/bin/*.dll
        )


    # Copy required DLLs to the bin directory. 
    # This ensures that every DLL file required can be accessed by 
    # the game

    # SDL
    file(COPY 
        ${PROJECT_SOURCE_DIR}/external/SDL2/${ARCH}/bin/SDL2.dll
        DESTINATION
        ${PROJECT_SOURCE_DIR}/bin/
    )
    # SDL Image
    file(COPY 
        ${SDL2_IMAGE_DLLS}
        DESTINATION
        ${PROJECT_SOURCE_DIR}/bin/
        )
    # SDL Mixer
    file(COPY 
        ${SDL2_MIXER_DLLS}
        DESTINATION
        ${PROJECT_SOURCE_DIR}/bin/
        )

endif ()


