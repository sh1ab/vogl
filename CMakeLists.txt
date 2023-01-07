cmake_minimum_required(VERSION 3.8)

project(sh_example)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

set(CMAKE_EXE_LINKER_FLAGS "-static")

add_subdirectory(example)
add_subdirectory(libs/sh_ogl)
add_subdirectory(libs/sh_key)
add_subdirectory(libs/sh_game)
add_subdirectory(libs/sh_vogl)