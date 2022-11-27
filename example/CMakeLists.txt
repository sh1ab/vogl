cmake_minimum_required(VERSION 3.8)

project(example)

add_executable(${PROJECT_NAME} main.cpp)


include_directories(../libs/sh_ogl)
include_directories(../libs/sh_key)
include_directories(../libs/sh_game)
include_directories(../libs/sh_vogl)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        sh_libs::sh_ogl_lib
        sh_libs::sh_key_lib
        sh_libs::sh_game_lib
        sh_libs::sh_vogl_lib
    )