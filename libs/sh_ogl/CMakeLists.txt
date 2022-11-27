cmake_minimum_required (VERSION 3.8)

project(sh_ogl_lib VERSION 1.0.0)

add_library(${PROJECT_NAME} 
    STATIC 
        "ogl.hpp"
        "sh_ogl.hpp"

        "abstract_ogl.hpp"
        
        "win_ogl.hpp"
        "win_ogl.cpp"

        "ppl_shad.hpp"
        "ppl_shad.cpp"

        "ubo.hpp"
    )

add_library(sh_libs::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

add_subdirectory(glad46)
include_directories(glad46)
    
target_include_directories(${PROJECT_NAME}
    PUBLIC 
        "ogl.hpp"
        "sh_ogl.hpp"
        "ppl_shad.hpp"
        "ubo.hpp"
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        glad46_lib
)