cmake_minimum_required(VERSION 3.8)

project(sh_key_lib VERSION 1.0.0)

add_library(${PROJECT_NAME}
    STATIC 
        "sh_key.hpp"
        "sh_key_lib_win.cpp"
    ) 
add_library(sh_libs::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
    "sh_key.hpp"
)