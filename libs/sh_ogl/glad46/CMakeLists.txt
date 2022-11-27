cmake_minimum_required (VERSION 3.8)

project (glad46_lib VERSION 1.0.0)

add_library(glad46_lib 
    STATIC 
        "include/glad/glad.h"
        "include/KHR/khrplatform.h"
        "src/glad.c"
    ) 

target_include_directories(glad46_lib
    PUBLIC 
        "include/"
    )   
target_link_libraries(glad46_lib
    PUBLIC
	    opengl32
	)