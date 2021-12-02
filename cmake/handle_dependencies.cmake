
include(FetchContent)

## Handle Dependencies for UNIX
if (UNIX)
    FetchContent_Declare(
        SDL2_CMAKE 
        GIT_REPOSITORY  https://github.com/aminosbh/sdl2-cmake-modules
        GIT_TAG         origin/master
        SOURCE_DIR      ${PROJECT_SOURCE_DIR}/external/cmake
    )
    
    FetchContent_MakeAvailable(SDL2_CMAKE)

    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/external/cmake"})

    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)

    include_directories(${PROJECT_NAME} 
        ${SDL2_INCLUDE_DIRS} 
        ${SDL2_IMAGE_INCLUDE_DIRS}
    )
    target_link_libraries(${PROJECT_NAME} 
        ${SDL2_LIBRARIES} 
        ${SDL2_IMAGE_LIBRARIES}
    )
endif ()

## Handle Dependencies for Windows
if (WIN32)
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
    
    FetchContent_MakeAvailable(SDL2 SDL2_image)
    set(ARCH x86_64-w64-mingw32)

    include_directories(
        ${PROJECT_NAME}
        ${PROJECT_SOURCE_DIR}/external/SDL2_image/${ARCH}/include/
        ${PROJECT_SOURCE_DIR}/external/SDL2/${ARCH}/include/
        ${PROJECT_SOURCE_DIR}/external/SDL2/${ARCH}/include/SDL2
        )
    target_link_libraries(
        ${PROJECT_NAME}
        ${PROJECT_SOURCE_DIR}/external/SDL2/${ARCH}/lib/libSDL2.dll.a
        ${PROJECT_SOURCE_DIR}/external/SDL2/${ARCH}/lib/libSDL2main.a
        ${PROJECT_SOURCE_DIR}/external/SDL2_image/${ARCH}/lib/libSDL2_image.dll.a
    )

    file(COPY 
        ${PROJECT_SOURCE_DIR}/external/SDL2_image/${ARCH}/bin/SDL2_image.dll
        DESTINATION
        ${PROJECT_SOURCE_DIR}/
    )
    file(COPY 
        ${PROJECT_SOURCE_DIR}/external/SDL2/${ARCH}/bin/SDL2.dll
        DESTINATION
        ${PROJECT_SOURCE_DIR}/
    )
    file(COPY 
        ${PROJECT_SOURCE_DIR}/external/SDL2_image/${ARCH}/bin/libpng16-16.dll
        DESTINATION
        ${PROJECT_SOURCE_DIR}/
    )
    file(COPY 
        ${PROJECT_SOURCE_DIR}/external/SDL2_image/${ARCH}/bin/zlib1.dll
        DESTINATION
        ${PROJECT_SOURCE_DIR}/
    )
endif ()


