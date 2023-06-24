cmake_minimum_required(VERSION 3.7)

project(Pacman LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(${PROJECT_NAME} "" main.c main.h fantome.c fantome.h pacman.h map.c map.h)
target_sources(${PROJECT_NAME} PRIVATE pacman.c)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE "SDL_MAIN_HANDLED")
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} m)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/pacman_sprites.bmp
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

IF (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
ENDIF()
